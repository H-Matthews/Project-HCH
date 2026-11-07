#include "core/StateStack/StateStack.hpp"

#include "core/Configuration/LoggerBuilder.hpp"
#include "core/StateStack/State.hpp"
#include "core/Application.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <cassert>

namespace Core {

StateStack::~StateStack() = default;

StateStack::StateStack(const ConfigSection* config, Context ctx)
    : mConfig(config), mStack(), mPendingRequests(), mContext(ctx), mLogger(nullptr) {
}

void StateStack::update(sf::Time fixedTimeStep) {
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->update(fixedTimeStep)) {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::draw() const {
    for (const auto& state : mStack) {
        state->draw();
    }
}

void StateStack::handleEvent(const Event& event) {
    for(auto stackIt = mStack.rbegin(); stackIt != mStack.rend(); ++stackIt) {
        if (!(*stackIt)->handleEvent(event)) {
            break;
        }
    }
}

void StateStack::popState() {
    mPendingRequests.emplace_back(Action::POP);
}

void StateStack::clearStates() {
    mPendingRequests.emplace_back(Action::CLEAR);
}

bool StateStack::isEmpty() const {
    return mStack.empty();
}

bool StateStack::isPendingListEmpty() const {
    return mPendingRequests.empty();
}

void StateStack::initializeLogger() {
    if constexpr (Utility::CAN_LOG) {
        if (!mLogger && mConfig) {
            mLogger = buildLogger(*mConfig);
        }
    }
}

std::unique_ptr<State>
StateStack::createState(const StateStack::PendingStateRequest& changeRequest) {
    std::unique_ptr<Core::State> createdState(changeRequest.stateConstructor());
    createdState->initializeState();
    return createdState;
}

void StateStack::applyPendingChanges() {
    std::string logMessage;

    for (StateStack::PendingStateRequest change : mPendingRequests) {
        if constexpr (Utility::CAN_LOG_INFO) {
            logMessage.clear();
            logMessage += "State Transition --> ";
        }

        switch (change.action) {
        case Action::PUSH: {
            mStack.push_back(createState(change));

            if constexpr (Utility::CAN_LOG_INFO) {
                logMessage += "PUSHING state: " + std::string(mStack.back()->getStateType());
            }

            break;
        }
        case Action::POP: {
            if constexpr (Utility::CAN_LOG_INFO) {
                logMessage += "REMOVING state: " + std::string(mStack[mStack.size() - 1]->getStateType());
            }

            mStack.pop_back();
            break;
        }
        case Action::CLEAR: {
            if constexpr (Utility::CAN_LOG_INFO) {
                logMessage += "CLEARING all states on stack";
            }

            mStack.clear();
            break;
        }
        }

        if constexpr (Utility::CAN_LOG_INFO) {
            if (mLogger) {
                mLogger->logInfo(logMessage);
            }
        }
    }

    mPendingRequests.clear();
}

StateStack::PendingStateRequest::PendingStateRequest(Action action)
    : action(action), stateConstructor() {}

} // NAMESPACE CORE
