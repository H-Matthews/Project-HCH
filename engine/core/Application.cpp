#include "core/Application.hpp"

#include "core/Window/WindowFactory.hpp"

// Unfortunately necessary for now
#include "core/Window/SfmlWindow.hpp"

#include "core/Configuration/LoggerBuilder.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

#include "utility/StringOperations.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>

namespace Core {

const sf::Time Application::TIME_PER_FRAME = sf::seconds(1.0f / 120.0f);

std::string convertAppStateEnumToString(const ApplicationState& appState) {
    switch (appState) {
    case ApplicationState::UNINITIALIZED:
        return "UNINITIALIZED";
    case ApplicationState::INITIALIZED:
        return "INITIALIZED";
    case ApplicationState::RUNNING:
        return "RUNNING";
    case ApplicationState::SHUTTING_DOWN:
        return "SHUTTING_DOWN";
    default:
        return "";
    }
}

std::optional<Core::ApplicationState> convertStringToAppStateEnum(std::string_view stringState) {
    std::string upperStateStr(stringState);

    Utility::toUpper(upperStateStr);

    std::optional<ApplicationState> appStateEnum = std::nullopt;

    if (upperStateStr == "UNINITIALIZED") {
        appStateEnum = ApplicationState::UNINITIALIZED;
    } else if (upperStateStr == "INITIALIZED") {
        appStateEnum = ApplicationState::INITIALIZED;
    } else if (upperStateStr == "RUNNING") {
        appStateEnum = ApplicationState::RUNNING;
    } else if (upperStateStr == "SHUTTING_DOWN") {
        appStateEnum = ApplicationState::SHUTTING_DOWN;
    }

    return appStateEnum;
}

Core::Application::Application(AppSpec appSpec)
    : mConfiguration(std::make_unique<Configuration>(std::move(appSpec.configSpec))),
      mDirectories(mConfiguration->rootDirectory(),
                   mConfiguration->getSection(Configuration::SECTION_NAME)),
      mState(ApplicationState::UNINITIALIZED), mTextures(), mFonts(),
      mNetwork(mConfiguration->getSection(MessageNetwork::SECTION_NAME)),
      mStateStack(*this, mConfiguration->getSection(StateStack::SECTION_NAME)) {
    if constexpr (Utility::CAN_LOG) {
        Utility::LogRegistry::instance()->configureRegistry(mDirectories.outputDirectory());

        if (auto appSection = mConfiguration->getSection(SECTION_NAME)) {
            mLogger = buildLogger(*appSection);
        }

        // NOTE: This call MUST be after LogRegistry::configureRegistry. If not, then the Logger
        //       May not get initialized correctly
        this->buildSubsystemLoggers();
    }

    // TODO: Make this configurable
    mWindow = WindowFactory::create(WindowBackend::SFML, std::move(appSpec.windowSpec));
}

bool Application::requestTransition(ApplicationState nextState) {
    if (!isValidTransition(mState, nextState)) {
        return false;
    }

    mState = nextState;

    return true;
}

void Application::initialize() {
    if (!mConfiguration) {
        throw ConfigurationException("Configuration is NULL");
    }

    if(!mWindow) {
        throw ConfigurationException("Window is NULL");
    }

    // Unfortunately necessary for now
    sfmlWindow = dynamic_cast<SfmlWindow*>((mWindow.get()));

    mWindow->setEventCallback([this](const sf::Event& event) {
        this->handleEvent(event);
    });

    requestTransition(ApplicationState::INITIALIZED);
}

sf::RenderWindow& Application::getRenderWindow() {
    if (!sfmlWindow) {
        throw ConfigurationException("Window backend is not SFML");
    }
    return sfmlWindow->getRenderWindow();
}

void Application::run() {
    if (!requestTransition(ApplicationState::RUNNING)) {
        std::string exceptionMessage =
            "Application is unable to transition to STATE:RUNNING; Currently: STATE:" +
            convertAppStateEnumToString(mState);

        if constexpr(Utility::CAN_LOG_ERROR) {
            if(mLogger) {
                mLogger->logError(exceptionMessage);
            }
        }

        throw ConfigurationException(exceptionMessage);
    }

    if constexpr (Utility::CAN_LOG_INFO) {
        if (mLogger) {
            mLogger->logInfo("Entering main RUN loop");
        }
    }

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow->isOpen()) {
        mWindow->pollEvents();

        // Clamp so a long stall (window drag, breakpoint) cannot cause a spiral of updates
        sf::Time elapsedTime = std::min(clock.restart(), sf::seconds(0.25f));
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TIME_PER_FRAME) {
            timeSinceLastUpdate -= TIME_PER_FRAME;

            processRealtimeInput();
            update(TIME_PER_FRAME);

            if (mStateStack.isEmpty()) {
                mWindow->close();

                if constexpr (Utility::CAN_LOG_INFO) {
                    if (mLogger) {
                        mLogger->logInfo("Closing Window....");
                    }
                }
            }
        }

        render();
    }

    if constexpr (Utility::CAN_LOG_INFO) {
        if (mLogger) {
            mLogger->logInfo("Exiting main RUN loop");
        }
    }
}

void Application::handleEvent(const sf::Event& event) {
    if(const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        mStateStack.handleKeyPressed(*keyPressed);
    }
    else if(const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        mStateStack.handleMouseMoved(*mouseMoved);
    }
    else if(event.is<sf::Event::Closed>()) {
        mWindow->close();
    }
}

void Application::processRealtimeInput() {
    mStateStack.handleRealTimeInput();

    // Notify subscribers of realtime input
    mNetwork.notifySubscribers();
}

void Application::update(sf::Time fixedTimeStep) {
    mStateStack.update(fixedTimeStep);
}

void Application::render() {

    // Clear old frame
    if(sfmlWindow) {
        sfmlWindow->clear(sf::Color(18, 18, 24));
    }

    // Draw game states
    mStateStack.draw();

    // Reset view
    if(sfmlWindow) {
        sfmlWindow->setView(sfmlWindow->getDefaultView());
    }

    // Swap buffers (Displays new drawings to screen)
    mWindow->swapBuffers();
}

void Application::buildSubsystemLoggers() {
    mNetwork.initializeLogger();
    mStateStack.initializeLogger();
}

bool Application::isValidTransition(ApplicationState from, ApplicationState to) const {
    switch (from) {
    case ApplicationState::UNINITIALIZED: {
        return to == ApplicationState::INITIALIZED;
    }
    case ApplicationState::INITIALIZED: {
        return to == ApplicationState::RUNNING;
    }
    case ApplicationState::RUNNING: {
        return to == ApplicationState::SHUTTING_DOWN;
    }
    default: {
        return false;
    }
    }
}

} // namespace Core