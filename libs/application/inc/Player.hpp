#pragma once

#include <SFML/Graphics.hpp>

#include "application/inc/Entity.hpp"
#include "application/inc/MessageNetwork/Messages/PlayerInputMessage.hpp"

#include "core/inc/Messaging/MessageNode.hpp"

namespace Application
{
    // Player is a subscriber to the PlayerInput Message
    class Player : public Application::Entity, public Core::MessageNode
    {
        public:
            Player(Core::MessageNetwork* network);
    
            // Derives from SceneNode
            void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    
        private:
            sf::CircleShape circleShape;

        private:
            void onNotify(Core::Message* message) override;
    };
}