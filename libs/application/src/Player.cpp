#include "application/inc/Player.hpp"

#include <iostream>

Application::Player::Player(Core::MessageNetwork* network) :
    MessageNode(network, "Player"),
    circleShape(50.f)
{
    // Set Subscriber Information
    MessageNode::subscribeTo(Messages::ID::PlayerActionMessage);
    MessageNode::registerSubscriberMessages();

    // set the shape color to green
    circleShape.setFillColor(sf::Color(100, 250, 50));

    circleShape.setPosition( {10.f, 50.f} );
    std::cout << circleShape.getPosition().x << " " << circleShape.getPosition().y << std::endl;
}

void Application::Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(circleShape, states);
}

void Application::Player::onNotify(Core::Message* message)
{
    PlayerInputMessage* playerInput = dynamic_cast<PlayerInputMessage*>( message );
    if(playerInput)
    {
        // Determine Action and Apply movement
        switch(playerInput->action)
        {
            case Actions::Player::NONE:
            {
                break;
            }
            case Actions::Player::MOVE_UP:
            {
                this->accelerate(0.f, 200.f);
                break;
            }
            case Actions::Player::MOVE_DOWN:
            {
                this->accelerate(0.f, -200.f);
                break;
            }
            case Actions::Player::MOVE_LEFT:
            {
                this->accelerate(-200.f, 0.f);
                break;
            }
            case Actions::Player::MOVE_RIGHT:
            {
                this->accelerate(200.f, 0.f);
                break;
            }
            case Actions::Player::JUMP:
            {
                break;
            }
        }
    }
}