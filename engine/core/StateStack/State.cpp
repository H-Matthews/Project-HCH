#include "core/StateStack/State.hpp"

#include "core/StateStack/StateStack.hpp"
#include "core/Application.hpp"

namespace Core {
State::State(StateStack& stack, Context ctx)
    : mStateStack(stack), mContext(ctx) {}

void State::requestStackPop() {
    mStateStack.popState();
}

void State::requestStateClear() {
    mStateStack.clearStates();
}
} // NAMESPACE CORE
