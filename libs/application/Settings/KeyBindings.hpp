#pragma once

#include "application/Data/PlayerAction.hpp"

#include "SFML/Window/Keyboard.hpp"

#include <map>

namespace Application
{
    /**
     * This class maintains the data mapping between a Key on the Keyboard and a Player Action
     */

    class KeyBindings
    {
      public:
        KeyBindings();

        void assignKey( Actions::Player action, sf::Keyboard::Scan scanCode );
        Actions::Player getAssignedAction( sf::Keyboard::Scan scanCode ) const;

        const std::map< sf::Keyboard::Scan, Actions::Player >& getKeyBindings();
        static bool isRealTimeAction( Actions::Player action );

      private:
        std::map< sf::Keyboard::Scan, Actions::Player > mKeyBindings;
    };
}