#include "core/StateStack/StateStack.hpp"

#include "core/Configuration/LoggerBuilder.hpp"
#include "core/StateStack/State.hpp"
#include "core/Application.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <cassert>

Core::StateStack::~StateStack() = default;

Core::StateStack::StateStack(Application& application, const ConfigSection* config)
    : mStack(), mPendingRequests(), mConfig(config), applicationRef(application), mLogger(nullptr) {
}

void Core::StateStack::update(sf::Time fixedTimeStep) {
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->update(fixedTimeStep))
            break;
    }

    applyPendingChanges();
}

void Core::StateStack::draw() {
    for (auto& state : mStack)
        state->draw();
}

void Core::StateStack::handleKeyPressed(const sf::Event::KeyPressed& keyPressed) {
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->handleKeyPressed(keyPressed))
            break;
    }
}

void Core::StateStack::handleMouseMoved(const sf::Event::MouseMoved& mouseMoved) {
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->handleMouseMoved(mouseMoved))
            break;
    }
}

void Core::StateStack::handleRealTimeInput() {
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->handleRealTimeInput())
            break;
    }
}

void Core::StateStack::popState() {
    mPendingRequests.push_back(PendingStateRequest(Action::POP));
}

void Core::StateStack::clearStates() {
    mPendingRequests.push_back(PendingStateRequest(Action::CLEAR));
}

bool Core::StateStack::isEmpty() const {
    return mStack.empty();
}

bool Core::StateStack::isPendingListEmpty() const {
    return mPendingRequests.empty();
}

Core::MessageNetwork* Core::StateStack::getMessageNetworkRef() {
    return applicationRef.getNetwork();
}

void Core::StateStack::initializeLogger() {
    if constexpr (Utility::CAN_LOG) {
        if (!mLogger && mConfig)
            mLogger = Core::buildLogger(*mConfig);
    }
}

std::unique_ptr<Core::State>
Core::StateStack::createState(const Core::StateStack::PendingStateRequest& changeRequest) {
    auto createdState(changeRequest.stateConstructor());
    createdState->setStackRef(this);
    createdState->initializeState();
    return createdState;
}

void Core::StateStack::applyPendingChanges() {
    std::string logMessage;

    for (Core::StateStack::PendingStateRequest change : mPendingRequests) {
        if constexpr (Utility::CAN_LOG_INFO) {
            logMessage.clear();
            logMessage += "State Transition --> ";
        }

        switch (change.action) {
        case Action::PUSH: {
            mStack.push_back(createState(change));

            if constexpr (Utility::CAN_LOG_INFO)
                logMessage += "PUSHING state: " + mStack.back()->getStateName();

            break;
        }
        case Action::POP: {
            if constexpr (Utility::CAN_LOG_INFO)
                logMessage += "REMOVING state: " + mStack[mStack.size() - 1]->getStateName();

            mStack.pop_back();
            break;
        }
        case Action::CLEAR: {
            if constexpr (Utility::CAN_LOG_INFO)
                logMessage += "CLEARING all states on stack";

            mStack.clear();
            break;
        }
        }

        if constexpr (Utility::CAN_LOG_INFO) {
            if (mLogger)
                mLogger->logInfo(logMessage);
        }
    }

    mPendingRequests.clear();
}

Core::StateStack::PendingStateRequest::PendingStateRequest(Action action)
    : action(action), stateConstructor() {}
