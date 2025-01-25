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

            void assignKey(PlayerAction action, sf::Keyboard::Key key);
            PlayerAction getAssignedAction(sf::Keyboard::Key key) const;

        private:
            static bool isRealTimeAction(PlayerAction action);

            std::map<sf::Keyboard::Key, PlayerAction> mKeyBindings;
    };
}