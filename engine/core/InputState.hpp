#pragma once

#include "core/Event.hpp"

#include <array>
#include <cstddef>
#include <variant>

namespace Core {

class InputState {
public:
    void update(const Event& event) {
        std::visit(Overloaded {
            [this](const KeyPressed& keyPressed) { mKeys[idx(keyPressed.key)] = true; },
            [this](const KeyReleased& keyReleased) { mKeys[idx(keyReleased.key)] = false; },
            [this](const MousePressed& mousePressed) { mButtons[idx(mousePressed.button)] = true; },
            [this](const MouseReleased& mouseRelease) { mButtons[idx(mouseRelease.button)] = false; },
            [this](const MouseMoved& mouseMoved) { mMouseX = mouseMoved.x; mMouseY = mouseMoved.y; },
            [this](const FocusLost&) { reset(); },
            [](const auto&) {}
        }, event);
    }

    bool isDown(Key key) const { return mKeys[idx(key)]; }
    bool isDown(MouseButton button) const { return mButtons[idx(button)]; }

    int getMouseX() const { return mMouseX; }
    int getMouseY() const { return mMouseY; }

private:
    template<typename E>
    static constexpr std::size_t idx(E e) { return static_cast<std::size_t>(e); }

    void reset() {
        mKeys.fill(false);
        mButtons.fill(false);
    }

    std::array<bool, static_cast<std::size_t>(Key::Count)> mKeys{};
    std::array<bool, static_cast<std::size_t>(MouseButton::Count)> mButtons{};

    int mMouseX = 0;
    int mMouseY = 0;
};

} // NAMESPACE CORE