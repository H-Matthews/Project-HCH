#pragma once

#include <SFML/Graphics.hpp>

#include "application/inc/SceneGraph/EntityNode.hpp"
#include "application/inc/MessageNetwork/Messages/PlayerInputMessage.hpp"

#include "core/inc/Messaging/MessageNode.hpp"

namespace Application
{
    // Player is a subscriber to the PlayerInput Message
    class PlayerNode : public Application::EntityNode, public Core::MessageNode
    {
        public:
            PlayerNode(Core::MessageNetwork& network);
    
            // Derives from SceneNode
            void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    
        private:
            sf::CircleShape circleShape;
            const float speed = 200.f;

        private:
            void onNotify(Core::Message* message) override;
    };
}