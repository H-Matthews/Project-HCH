#pragma once

#include <SFML/Graphics.hpp>

#include "core/inc/Messaging/MessageNode.hpp"

#include "application/inc/MessageNetwork/Messages/PlayerInputMessage.hpp"
#include "application/inc/KeyBindings.hpp"

namespace Application
{

    /*
        Publishes the following Messages:
            1. PlayerInput Message

        The Publisher has keyBindings that it references when receiving a keyPressedEvent OR real time input
        IF the sf::Scancode is registered to an action in the keyBindings, then we Publisher a Playerinput message
    */
    class PlayerInputPublisher : public Core::MessageNode
    {
        public:
            PlayerInputPublisher(Core::MessageNetwork& network, Application::KeyBindings keyBindings);
    
            void handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent);
            void handleRealTimeInput();
    
        private:
            Application::KeyBindings mKeyBindings;

            std::unique_ptr<Application::PlayerInputMessage> mPlayerActionMessage;
    };
}