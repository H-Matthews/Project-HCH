#pragma once

#include "core/InputState.hpp"

namespace Core {

/**
 * A State's read-only window onto the shared InputState.
 *
 * There is ONE InputState (the real key/button state); every State owns its own InputView of it.
 * While a state above blocks input (State::blocksInput), the StateStack disables this view and
 * isDown(), wasPressed() and wasReleased() all report false, so polled input is blocked the same
 * way events are.
 *
 * The mouse position is not gated: it carries no "press" and is harmless to read (hover effects).
 */
class InputView {
  public:
    explicit InputView(const InputState& input) : mInput(&input) {}

    bool isDown(Key key) const { return mEnabled && mInput->isDown(key); }
    bool isDown(MouseButton button) const { return mEnabled && mInput->isDown(button); }

    bool wasPressed(Key key) const { return mEnabled && mInput->wasPressed(key); }
    bool wasPressed(MouseButton button) const { return mEnabled && mInput->wasPressed(button); }

    bool wasReleased(Key key) const { return mEnabled && mInput->wasReleased(key); }
    bool wasReleased(MouseButton button) const { return mEnabled && mInput->wasReleased(button); }

    int getMouseX() const { return mInput->getMouseX(); }
    int getMouseY() const { return mInput->getMouseY(); }

    // Called by StateStack only
    void setEnabled(bool enabled) { mEnabled = enabled; }

  private:
    const InputState* mInput;
    bool mEnabled = true;
};

} // NAMESPACE CORE
