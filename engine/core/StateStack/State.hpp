#pragma once

#include "core/StateStack/InputView.hpp"
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

    virtual void handleEvent(const Event& event) = 0;
    virtual void update(sf::Time fixedTimeStep) = 0;
    virtual void draw() = 0;

    // What this state lets through to the states BELOW it (see the rule in StateStack.hpp).
    // Override in overlays such as pause or game-over screens.
    virtual bool blocksInput() const { return false; }   // no events, no held keys below
    virtual bool blocksUpdates() const { return false; } // states below are not updated (still drawn)

    // Called by StateStack each tick; not for state code
    void setInputEnabled(bool enabled) { mInput.setEnabled(enabled); }

    virtual std::string_view getStateType() = 0;

protected:
    // Held keys/buttons. Reports "up" while a state above blocks input.
    const InputView& input() const { return mInput; }
    Window& window() const { return mContext.window; }
    sf::RenderWindow& renderWindow() const;
    FontHolder& fonts() const { return mContext.fonts; }
    TextureHolder& textures() const { return mContext.textures; }

    template <typename TState>
    requires(std::is_base_of_v<State, TState> && std::constructible_from<TState, StateStack&, Context>)
    void requestStackPush();
    void requestStackPop();
    void requestStateClear();

private:
    StateStack& mStateStack;
    Context mContext;
    InputView mInput;
};

template <typename TState>
requires(std::is_base_of_v<State, TState> && std::constructible_from<TState, StateStack&, Context>)
void State::requestStackPush() {
    mStateStack.pushState<TState>();
}

} // namespace Core
