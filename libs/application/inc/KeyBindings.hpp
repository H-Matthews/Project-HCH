#pragma once

#include "application/inc/PlayerAction.hpp"

#include "SFML/Window/Keyboard.hpp"

#include <map>

namespace Application
{
    class KeyBindings
    {
        public:
            KeyBindings();

            void assignKey(PlayerAction action, sf::Keyboard::Scan scanCode);
            PlayerAction getAssignedAction(sf::Keyboard::Scan scanCode) const;

        private:
            static bool isRealTimeAction(PlayerAction action);

            std::map<sf::Keyboard::Scan, PlayerAction> mKeyBindings;
    };
}