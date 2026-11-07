#pragma once

#include "core/Event.hpp"

#include <array>
#include <cstddef>
#include <variant>

namespace Core {

/**
 * The shared, polled view of the keyboard and mouse, built from events.
 *
 *  - isDown(x)       "held" (a level): true for as long as the key/button is down
 *  - wasPressed(x)   "edge": true for the one update after the key/button went down
 *  - wasReleased(x)  "edge": true for the one update after the key/button went up
 *
 * Edges are cleared by clearEdges(), which the Application calls once per fixed update after the
 * states have run. A press and release inside the same update gives wasPressed and wasReleased
 * both true while isDown is false, so quick taps are never lost.
 */
class InputState {
  public:
    void update(const Event& event) {
        std::visit(Overloaded{[this](const KeyPressed& keyPressed) {
                                  mKeys[idx(keyPressed.key)] = true;
                                  if (!keyPressed.repeat)
                                      mKeysPressed[idx(keyPressed.key)] = true;
                              },
                              [this](const KeyReleased& keyReleased) {
                                  mKeys[idx(keyReleased.key)] = false;
                                  mKeysReleased[idx(keyReleased.key)] = true;
                              },
                              [this](const MousePressed& mousePressed) {
                                  mButtons[idx(mousePressed.button)] = true;
                                  mButtonsPressed[idx(mousePressed.button)] = true;
                              },
                              [this](const MouseReleased& mouseRelease) {
                                  mButtons[idx(mouseRelease.button)] = false;
                                  mButtonsReleased[idx(mouseRelease.button)] = true;
                              },
                              [this](const MouseMoved& mouseMoved) {
                                  mMouseX = mouseMoved.x;
                                  mMouseY = mouseMoved.y;
                              },
                              [this](const FocusLost&) { reset(); }, [](const auto&) {}},
                   event);
    }

    // Call once per fixed update, after the states have read the edges
    void clearEdges() {
        mKeysPressed.fill(false);
        mKeysReleased.fill(false);
        mButtonsPressed.fill(false);
        mButtonsReleased.fill(false);
    }

    bool isDown(Key key) const { return mKeys[idx(key)]; }
    bool isDown(MouseButton button) const { return mButtons[idx(button)]; }

    bool wasPressed(Key key) const { return mKeysPressed[idx(key)]; }
    bool wasPressed(MouseButton button) const { return mButtonsPressed[idx(button)]; }

    bool wasReleased(Key key) const { return mKeysReleased[idx(key)]; }
    bool wasReleased(MouseButton button) const { return mButtonsReleased[idx(button)]; }

    int getMouseX() const { return mMouseX; }
    int getMouseY() const { return mMouseY; }

  private:
    template <typename E> static constexpr std::size_t idx(E e) {
        return static_cast<std::size_t>(e);
    }

    void reset() {
        mKeys.fill(false);
        mButtons.fill(false);
        clearEdges();
    }

    using KeyFlags = std::array<bool, static_cast<std::size_t>(Key::Count)>;
    using ButtonFlags = std::array<bool, static_cast<std::size_t>(MouseButton::Count)>;

    KeyFlags mKeys{};
    ButtonFlags mButtons{};

    KeyFlags mKeysPressed{};
    KeyFlags mKeysReleased{};
    ButtonFlags mButtonsPressed{};
    ButtonFlags mButtonsReleased{};

    int mMouseX = 0;
    int mMouseY = 0;
};

} // namespace Core
