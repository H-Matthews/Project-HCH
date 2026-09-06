#include "core/Application.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <iostream>

const sf::Time Core::Application::TIME_PER_FRAME = sf::seconds(1.0f / 120.0f);
const std::string Core::Application::TYPE_NAME = "Application";

Core::Application::Application(Core::ConfigSpec configSpec)
    : Configurable(TYPE_NAME), mConfiguration(std::make_unique<Core::Configuration>(configSpec)),
      mState(State::NONE), mTextures(), mNetwork(), mStateStack(*this),
      mWindow(sf::VideoMode({640, 480}), "Application Window", sf::Style::Close) {
    // Must call back up to the Configurable
    std::shared_ptr<ConfigNode> rootNode = ConfigurationTree::instance()->getRootNode();
    std::vector<std::shared_ptr<Core::ConfigNode>> childrenNodes = rootNode->getChildren();
    if (childrenNodes.empty())
        throw ConfigurationException("Children were not populated for RootNode");

    Configurable::configure(rootNode);

    return;
}

void Core::Application::initialize() {
    if (!mConfiguration)
        throw ConfigurationException("Configuration is NULL");

    loadResources();

    transitionState(State::INITIALIZED);

    return;
}

void Core::Application::run() {
    if (!transitionState(State::RUNNING)) {
        std::string exceptionMessage =
            "Application is unable to transition to STATE:RUNNING; Currently: STATE:" +
            convertAppStateEnumToString(mState);

        if (isInitialized()) {
            exceptionMessage += " The application requires a State to be pushed onto the Stack";
        } else {
            exceptionMessage += " The Application is NOT initialized, call initialize()";
        }

        if constexpr (Utility::CAN_LOG) {
            if (mLogger)
                mLogger->logError(exceptionMessage);
        }

        throw ConfigurationException(exceptionMessage.c_str());
    }

    if constexpr (Utility::CAN_LOG)
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

                if constexpr (Utility::CAN_LOG)
                    mLogger->logInfo("Closing Window....");
            }
        }

        render();
    }

    if constexpr (Utility::CAN_LOG)
        mLogger->logInfo("Exiting main RUN loop");
}

void Core::Application::processInput() {
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

void Core::Application::update(sf::Time fixedTimeStep) {
    mStateStack.update(fixedTimeStep);
}

void Core::Application::render() {
    mWindow.clear(sf::Color::Cyan);

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}

void Core::Application::loadResources() {
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

bool Core::Application::transitionState(State statusToTransfer) {
    bool retStatus = false;

    if (mState == statusToTransfer)
        return retStatus;

    using enum Core::Application::State;

    switch (statusToTransfer) {
    case NONE: {
        // DO NOTHING

        break;
    }
    case INITIALIZED: {
        if (mState == NONE) {
            mState = statusToTransfer;
            retStatus = true;
        }

        break;
    }
    case RUNNING: {
        if (mState == INITIALIZED && !mStateStack.isPendingListEmpty()) {
            mState = statusToTransfer;
            retStatus = true;
        }

        break;
    }
    case SHUTTING_DOWN: {
        if (mState == RUNNING) {
            mState = statusToTransfer;
            retStatus = true;
        }

        break;
    }
    }

    return retStatus;
}

std::string Core::Application::convertAppStateEnumToString(const State& state) const {
    std::string retString;

    using enum Core::Application::State;

    switch (state) {
    case NONE: {
        retString = "NONE";

        break;
    }
    case INITIALIZED: {
        retString = "INITIALIZED";

        break;
    }
    case RUNNING: {
        retString = "RUNNING";

        break;
    }
    case SHUTTING_DOWN: {
        retString = "SHUTTING_DOWN";

        break;
    }
    }

    return retString;
}

Core::Application::State
Core::Application::convertStringToAppStateEnum(std::string_view stringState) const {
    using enum Core::Application::State;

    State retState = NONE;

    if (stringState == "INITIALIZED") {
        retState = INITIALIZED;
    } else if (stringState == "RUNNING") {
        retState = RUNNING;
    } else if (stringState == "SHUTTING_DOWN") {
        retState = SHUTTING_DOWN;
    }

    return retState;
}