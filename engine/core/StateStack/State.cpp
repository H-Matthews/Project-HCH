#include "core/StateStack/State.hpp"

#include "core/StateStack/StateStack.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

namespace Core {
State::State(StateStack& stack, Context ctx)
    : mStateStack(stack), mContext(ctx) {}

sf::RenderWindow& State::renderWindow() const {
    if (!mContext.renderWindow) {
        throw ConfigurationException("Window backend is not SFML");
    }

    return *mContext.renderWindow;
}

void State::requestStackPop() {
    mStateStack.popState();
}

void State::requestStateClear() {
    mStateStack.clearStates();
}
} // NAMESPACE CORE
