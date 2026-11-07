#include "core/Event.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <optional>

namespace Core {
namespace {

using SfKey = sf::Keyboard::Key;
using SfButton = sf::Mouse::Button;

// sf::Keyboard::Key is a scoped enum and therefore doesn't support basic arithmetic
// This function fixes that by explicitly casting to int
constexpr int offset(SfKey k, SfKey base) {
    return static_cast<int>(k) - static_cast<int>(base);
}

// Range-base mapping below relies on SFML declaring these runs contiguously
// and on our enum declaring the same runs in the same order. Fail loudly if not.
static_assert(static_cast<int>(SfKey::Z) - static_cast<int>(SfKey::A) ==
              static_cast<int>(Key::Z) - static_cast<int>(Key::A));

static_assert(static_cast<int>(SfKey::Num9) - static_cast<int>(SfKey::Num0) ==
              static_cast<int>(Key::Num9) - static_cast<int>(Key::Num0));

static_assert(static_cast<int>(SfKey::F12) - static_cast<int>(SfKey::F1) ==
              static_cast<int>(Key::F12) - static_cast<int>(Key::F1));

Key toKey(SfKey sfKey) {
    // Contiguous Runs
    if (sfKey >= SfKey::A && sfKey <= SfKey::Z) {
        return static_cast<Key>(static_cast<int>(Key::A) + offset(sfKey, SfKey::A));
    }
    if (sfKey >= SfKey::Num0 && sfKey <= SfKey::Num9) {
        return static_cast<Key>(static_cast<int>(Key::Num0) + offset(sfKey, SfKey::Num0));
    }
    if (sfKey >= SfKey::F1 && sfKey <= SfKey::F12) {
        return static_cast<Key>(static_cast<int>(Key::F1) + offset(sfKey, SfKey::F1));
    }

    // Everything else, one by one
    switch (sfKey) {
        case SfKey::Left:      return Key::Left;
        case SfKey::Right:     return Key::Right;
        case SfKey::Up:        return Key::Up;
        case SfKey::Down:      return Key::Down;
 
        case SfKey::Escape:    return Key::Escape;
        case SfKey::Space:     return Key::Space;
        case SfKey::Enter:     return Key::Enter;
        case SfKey::Backspace: return Key::Backspace;
        case SfKey::Tab:       return Key::Tab;
 
        case SfKey::Insert:    return Key::Insert;
        case SfKey::Delete:    return Key::Delete;
        case SfKey::Home:      return Key::Home;
        case SfKey::End:       return Key::End;
        case SfKey::PageUp:    return Key::PageUp;
        case SfKey::PageDown:  return Key::PageDown;
 
        case SfKey::LShift:    return Key::LShift;
        case SfKey::RShift:    return Key::RShift;
        case SfKey::LControl:  return Key::LControl;
        case SfKey::RControl:  return Key::RControl;
        case SfKey::LAlt:      return Key::LAlt;
        case SfKey::RAlt:      return Key::RAlt;
        case SfKey::LSystem:   return Key::LSystem;
        case SfKey::RSystem:   return Key::RSystem;

        default:               return Key::Unknown;
    }
}

MouseButton toMouse(sf::Mouse::Button sfButton) {
    switch(sfButton) {
        case SfButton::Left:   return MouseButton::Left;
        case SfButton::Right:  return MouseButton::Right;
        case SfButton::Middle: return MouseButton::Middle;
        case SfButton::Extra1: return MouseButton::Extra1;
        case SfButton::Extra2: return MouseButton::Extra2;
        default:               return MouseButton::Left; // Should be unreachable
    }
}

} // NAMESPACE ANONYMOUS

// Return std::nullopt for events we don't care about
std::optional<Event> translate(const sf::Event& e) {
    using SfEvent = sf::Event;

    if(e.is<SfEvent::Closed>())         return WindowClosed{};
    if(e.is<SfEvent::FocusGained>())    return FocusGained{};
    if(e.is<SfEvent::FocusLost>())      return FocusLost{};
    if(e.is<SfEvent::MouseEntered>())   return MouseEntered{};
    if(e.is<SfEvent::MouseLeft>())      return MouseLeft{};

    if(auto* sfR = e.getIf<SfEvent::Resized>()) {
        return WindowResized{sfR->size.x, sfR->size.y};
    }

    if(auto* sfK = e.getIf<SfEvent::KeyPressed>()) {
        return KeyPressed{
            toKey(sfK->code),
            {sfK->shift, sfK->control, sfK->alt, sfK->system},
            false // SFML3 gives no repeat flag
        }; 
    }

    if(auto* sfK = e.getIf<SfEvent::KeyReleased>()) {
        return KeyReleased{
            toKey(sfK->code),
            {sfK->shift, sfK->control, sfK->alt, sfK->system}
        };
    }

    if(auto* sfT = e.getIf<SfEvent::TextEntered>()) {
        return TextEntered{sfT->unicode};
    }

    if(auto* sfM = e.getIf<SfEvent::MouseMoved>()) {
        return MouseMoved{sfM->position.x, sfM->position.y};
    }

    if(auto* sfM = e.getIf<SfEvent::MouseButtonPressed>()) {
        return MousePressed{toMouse(sfM->button), sfM->position.x, sfM->position.y};
    }

    if(auto* sfM = e.getIf<SfEvent::MouseButtonReleased>()) {
        return MouseReleased{toMouse(sfM->button), sfM->position.x, sfM->position.y};
    }

    if(auto* sfM = e.getIf<SfEvent::MouseWheelScrolled>()) {
        if(sfM->wheel == sf::Mouse::Wheel::Vertical) {
            return MouseWheel{sfM->delta, sfM->position.x, sfM->position.y};
        }

        // Ignoring Horizontal Wheel for now
        return std::nullopt;
    }

    return std::nullopt;
}

} // NAMESPACE CORE

