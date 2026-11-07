#include "core/StateStack/StateStack.hpp"

#include "core/Configuration/LoggerBuilder.hpp"
#include "core/StateStack/State.hpp"
#include "core/Application.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <cassert>

namespace Core {

StateStack::~StateStack() = default;

StateStack::StateStack(const ConfigSection* config, Context ctx) : mConfig(config), mContext(ctx) {}

void StateStack::refreshInputGating() {
    bool inputOpen = true;

    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        (*itr)->setInputEnabled(inputOpen);

        if ((*itr)->blocksInput()) {
            inputOpen = false;
        }
    }
}

void StateStack::update(sf::Time fixedTimeStep) {
    refreshInputGating();

    bool updatesOpen = true;

    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (updatesOpen) {
            (*itr)->update(fixedTimeStep);
        }

        if ((*itr)->blocksUpdates()) {
            updatesOpen = false;
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
    refreshInputGating();

    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        (*itr)->handleEvent(event);

        if ((*itr)->blocksInput()) {
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

    // Take the queue first: a state may request more changes from initializeState(), and those
    // must not be added to the vector being iterated. They wait for the next update.
    const std::vector<PendingStateRequest> requests = std::move(mPendingRequests);
    mPendingRequests.clear();

    for (const PendingStateRequest& change : requests) {
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
            if (mStack.empty()) {
                if constexpr (Utility::CAN_LOG_WARN) {
                    if (mLogger) {
                        mLogger->logWarn("State Transition --> ignored POP on an empty stack");
                    }
                }

                continue;
            }

            if constexpr (Utility::CAN_LOG_INFO) {
                logMessage += "REMOVING state: " + std::string(mStack.back()->getStateType());
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
}

StateStack::PendingStateRequest::PendingStateRequest(Action action)
    : action(action), stateConstructor() {}

} // namespace Core
