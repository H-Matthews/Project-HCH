#include "application/Scene/ECS/Systems/PlayerInputSystem.hpp"

#include "application/Scene/ECS/Components/PlayerInput.hpp"

System::PlayerInputSystem::PlayerInputSystem( Core::MessageNetwork& network ) :
    Core::MessageNode( network, "PlayerInput System", Core::NodeType::SUBSCRIBER )
{
    // Set Subscriber Topics
    MessageNode::addTopic( Messages::ID::PlayerActionMessage );
}

void System::PlayerInputSystem::onNotify( Core::Message* message )
{
    Application::PlayerInputMessage* playerInput = dynamic_cast< Application::PlayerInputMessage* >( message );
    if ( playerInput )
    {
        // Grab PlayerInput Component data
        auto view = mRegistry->view< Component::PlayerInput >();
        auto& playerInputData = view.get< Component::PlayerInput >( *( view.begin() ) );

        // Modify PlayerInputData
        switch ( playerInput->action )
        {
            case Actions::Player::NONE:
            {
                break;
            }
            case Actions::Player::MOVE_UP:
            {
                playerInputData.mPlayerVelocity.y -= playerInputData.SPEED;
                break;
            }
            case Actions::Player::MOVE_DOWN:
            {
                playerInputData.mPlayerVelocity.y += playerInputData.SPEED;
                break;
            }
            case Actions::Player::MOVE_LEFT:
            {
                playerInputData.mPlayerVelocity.x -= playerInputData.SPEED;
                break;
            }
            case Actions::Player::MOVE_RIGHT:
            {
                playerInputData.mPlayerVelocity.x += playerInputData.SPEED;
                break;
            }
            case Actions::Player::JUMP:
            {
                break;
            }
        }
    }

    return;
}
