#include "core/Application.hpp"

#include "core/Configuration/LoggerBuilder.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

#include "utility/StringOperations.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include <SFML/Graphics.hpp>

namespace Core {

const sf::Time Application::TIME_PER_FRAME = sf::seconds(1.0f / 120.0f);

std::string convertAppStateEnumToString(const ApplicationState& appState) {
    switch(appState) {
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

    if(upperStateStr == "UNINITIALIZED") {
        appStateEnum = ApplicationState::UNINITIALIZED;
    }
    else if(upperStateStr == "INITIALIZED") {
        appStateEnum = ApplicationState::INITIALIZED;
    }
    else if(upperStateStr == "RUNNING") {
        appStateEnum = ApplicationState::RUNNING;
    }
    else if(upperStateStr == "SHUTTING_DOWN") {
        appStateEnum = ApplicationState::SHUTTING_DOWN;
    }

    return appStateEnum;
}

Core::Application::Application(ConfigSpec configSpec)
    : mConfiguration(std::make_unique<Configuration>(std::move(configSpec))),
      mDirectories(mConfiguration->getSection(Configuration::SECTION_NAME)), mState(ApplicationState::UNINITIALIZED),
      mTextures(), mNetwork(mConfiguration->getSection(MessageNetwork::SECTION_NAME)),
      mStateStack(*this, mConfiguration->getSection(StateStack::SECTION_NAME)),
      mWindow(sf::VideoMode({640, 480}), "Application Window", sf::Style::Close) {
    if constexpr (Utility::CAN_LOG) {
        Utility::LogRegistry::instance()->configureRegistry(mDirectories.outputDirectory());

        if (auto appSection = mConfiguration->getSection(SECTION_NAME))
            mLogger = buildLogger(*appSection);

        // NOTE: This call MUST be after LogRegistry::configureRegistry. If not, then the Logger
        //       May not get initialized correctly
        this->buildSubsystemLoggers();
    }
}

bool Application::requestTransition(ApplicationState nextState) {
    if (!isValidTransition(mState, nextState)) {
        // LOG HERE
        return false;
    }

    ApplicationState prev = mState;
    mState = nextState;

    // Notify state change through event system
    // mMessageNetwork.broadcast(EngineStatusChangedEvent{prev, mstate});

    return true;
}

void Application::initialize() {
    if (!mConfiguration)
        throw ConfigurationException("Configuration is NULL");

    loadResources();

    requestTransition(ApplicationState::INITIALIZED);
}

void Application::run() {
    // if (!transitionState(State::RUNNING)) {
    //     std::string exceptionMessage =
    //         "Application is unable to transition to STATE:RUNNING; Currently: STATE:" +
    //         convertAppStateEnumToString(mState);

    //     if (isInitialized()) {
    //         exceptionMessage += " The application requires a State to be pushed onto the Stack";
    //     } else {
    //         exceptionMessage += " The Application is NOT initialized, call initialize()";
    //     }

    //     if constexpr (Utility::CAN_LOG_ERROR) {
    //         if (mLogger)
    //             mLogger->logError(exceptionMessage);
    //     }

    //     throw ConfigurationException(exceptionMessage);
    // }

    if (!requestTransition(ApplicationState::RUNNING)) {
        std::string exceptionMessage =
            "Application is unable to transition to STATE:RUNNING; Currently: STATE:" +
            convertAppStateEnumToString(mState);
    }

    if constexpr (Utility::CAN_LOG_INFO)
        mLogger->logInfo("Entering main RUN loop");

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TIME_PER_FRAME) {
            timeSinceLastUpdate -= TIME_PER_FRAME;

            processInput();
            update(TIME_PER_FRAME);

            if (mStateStack.isEmpty()) {
                mWindow.close();

                if constexpr (Utility::CAN_LOG_INFO)
                    mLogger->logInfo("Closing Window....");
            }
        }

        render();
    }

    if constexpr (Utility::CAN_LOG_INFO)
        mLogger->logInfo("Exiting main RUN loop");
}

void Application::processInput() {
    // SFMLs Window Class will detect events and then call these functions if the event matches
    // When needed, Add Event Subtypes here

    mWindow.handleEvents([this](const sf::Event::Closed&) { mWindow.close(); },
                         [this](const sf::Event::KeyPressed& keyPressedEvent) {
                             mStateStack.handleKeyPressed(keyPressedEvent);
                         },
                         [this](const sf::Event::MouseMoved& mouseMovedEvent) {
                             mStateStack.handleMouseMoved(mouseMovedEvent);
                         });

    mStateStack.handleRealTimeInput();

    mNetwork.notifySubscribers();
}

void Application::update(sf::Time fixedTimeStep) {
    mStateStack.update(fixedTimeStep);
}

void Application::render() {
    mWindow.clear(sf::Color::Cyan);

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}

void Application::loadResources() {
    // auto fontTexturePaths = mConfiguration->getAssetPaths();
    // std::string texturePath = fontTexturePaths.first;
    // std::string fontPath = fontTexturePaths.second;

    // TODO: This needs to be reexamined. Shouldn't hardcode file names like this
    // Should tie this to the config file...
    // mTextures.load( Textures::ID::PLAYER, texturePath + "/" + "playerSprite.png" );
    // mTextures.load( Textures::ID::BACKGROUND, texturePath + "/" + "background.png" );
    // mTextures.load( Textures::ID::ENEMY, texturePath + "/" + "enemySprite.png" );

    return;
}

void Application::buildSubsystemLoggers() {
    mNetwork.initializeLogger();
    mStateStack.initializeLogger();
}

bool Application::isValidTransition(ApplicationState from, ApplicationState to) const {
    switch(from) {
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

} // NAMESPACE CORE