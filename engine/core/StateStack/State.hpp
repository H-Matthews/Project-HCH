#pragma once

#include "core/Messaging/MessageNetwork.hpp"
#include "core/StateStack/StateStack.hpp"

#include "core/GameAssetContainer/ResourceTypes.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>

#include <string>

namespace Core {

class Window;

class State {
public:
    explicit State(StateStack& stack, Context ctx);
    virtual ~State() = default;

    // To be called after setting the StateStack reference
    virtual bool initializeState() { return true; }

    virtual bool handleEvent(const Event& event) = 0;
    virtual bool update(sf::Time fixedTimeStep) = 0;
    virtual void draw() = 0;

    virtual std::string_view getStateType() = 0;

protected:
    const InputState& input() const { return mContext.input; }
    Window& window() const { return mContext.window; }
    sf::RenderWindow& renderWindow() const;

    template <typename TState>
    requires(std::is_base_of_v<State, TState> && std::constructible_from<TState, StateStack&, Context>)
    void requestStackPush();
    void requestStackPop();
    void requestStateClear();

private:
    StateStack& mStateStack;
    Context mContext;
};

template <typename TState>
requires(std::is_base_of_v<State, TState> && std::constructible_from<TState, StateStack&, Context>)
void State::requestStackPush() {
    mStateStack.pushState<TState>();
}

} // namespace Core
