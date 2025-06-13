#include "application/Messaging/Messages/PlayerInputMessage.hpp"

Application::PlayerInputMessage::PlayerInputMessage( const Messages::ID messageID ) :
    Message( messageID, "PlayerActionMessage" ),
    action( Actions::Player::NONE )
{}

Application::PlayerInputMessage* Application::PlayerInputMessage::clone() const
{
    return new PlayerInputMessage( *this );
}