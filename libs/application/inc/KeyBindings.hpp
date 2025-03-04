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

            void assignKey(Actions::Player action, sf::Keyboard::Scan scanCode);
            Actions::Player getAssignedAction(sf::Keyboard::Scan scanCode) const;

            const std::map<sf::Keyboard::Scan, Actions::Player>& getKeyBindings();
            static bool isRealTimeAction(Actions::Player action);

        private:
            std::map<sf::Keyboard::Scan, Actions::Player> mKeyBindings;
    };
}