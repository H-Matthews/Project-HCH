#pragma once

#include <SFML/Graphics.hpp>

#include "core/inc/Messaging/MessageNode.hpp"

#include "application/inc/Messages/PlayerInputMessage.hpp"
#include "application/inc/KeyBindings.hpp"

namespace Application
{

    /* Publishes the following Messages:
        1. PlayerInputMessage
    */
    class PlayerInputPublisher : public Core::MessageNode
    {
        public:
            PlayerInputPublisher(Core::MessageNetwork* network, Application::KeyBindings keyBindings);
    
            void handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent);
    
        private:
            Application::KeyBindings mKeyBindings;
            std::shared_ptr<Application::PlayerInputMessage> mPlayerActionMessage;
    };
}