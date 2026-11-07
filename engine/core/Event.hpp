#pragma once

#include <cstdint>
#include <variant>

namespace Core {

enum class Key : std::uint16_t {
    Unknown = 0,

    // Letters
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    // Number Row
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    // Arrows
    Left,
    Right,
    Up,
    Down,

    // Common Controls
    Escape,
    Space,
    Enter,
    Backspace,
    Tab,
    Insert,
    Delete,
    Home,
    End,
    PageUp,
    PageDown,

    // Modifiers (as keys)
    LShift,
    RShift,
    LControl,
    RControl,
    LAlt,
    RAlt,
    LSystem,
    RSystem,

    // Function Keys
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    // Should always be last
    Count
};

enum class MouseButton : std::uint8_t {
    Left,
    Right,
    Middle,
    Extra1,
    Extra2,
    Count // Should always be last
};

struct Modifiers {
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool system = false; // Windows/Command/Super
};

struct KeyPressed {
    Key key;
    Modifiers mods;

    // True for OS auto-repeat
    bool repeat = false;
};

struct KeyReleased {
    Key key;
    Modifiers mods;
};

// Layout- and shift-aware text input. Use for UI text fields, not for controls.
struct TextEntered {
    char32_t codepoint;
};

// Raw window pixels
struct MouseMoved {
    int x;
    int y;
};

struct MousePressed {
    MouseButton button;
    int x;
    int y;
};

struct MouseReleased {
    MouseButton button;
    int x;
    int y;
};

struct MouseWheel {
    float delta; // Positive = scroll up/away from user
    int x;
    int y;
};

struct MouseEntered {};
struct MouseLeft {};

struct WindowResized {
    unsigned width;
    unsigned height;
};

struct WindowClosed {};
struct FocusGained {};
struct FocusLost {};

using Event = std::variant<KeyPressed, KeyReleased, TextEntered, MouseMoved, MousePressed,
                           MouseReleased, MouseWheel, MouseEntered, MouseLeft, WindowResized,
                           WindowClosed, FocusGained, FocusLost>;

template <class... Ts> struct Overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

} // namespace Core