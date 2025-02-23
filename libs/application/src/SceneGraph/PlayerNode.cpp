#include "application/inc/SceneGraph/PlayerNode.hpp"

Application::PlayerNode::PlayerNode(Core::MessageNetwork& network) :
    Core::MessageNode(network, "Player Controller", Core::NodeType::SUBSCRIBER),
    circleShape(30.f)
{
    // Set Subscriber Topics
    MessageNode::addTopic(Messages::ID::PlayerActionMessage);

    // set the shape color to green
    circleShape.setFillColor(sf::Color(100, 250, 50));

    sf::FloatRect bounds = circleShape.getLocalBounds();
    circleShape.setOrigin({bounds.size.x, bounds.size.y});
}

void Application::PlayerNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(circleShape, states);
}

void Application::PlayerNode::onNotify(Core::Message* message)
{
    PlayerInputMessage* playerInput = dynamic_cast<PlayerInputMessage*>( message );
    if(playerInput)
    {
        sf::Vector2f playerVector(0.f, 0.f);
        // Determine Action and Apply movement
        switch(playerInput->action)
        {
            case Actions::Player::NONE:
            {
                break;
            }
            case Actions::Player::MOVE_UP:
            {
                playerVector.y -= speed;
                break;
            }
            case Actions::Player::MOVE_DOWN:
            {
                playerVector.y += speed;
                break;
            }
            case Actions::Player::MOVE_LEFT:
            {
                playerVector.x -= speed;
                break;
            }
            case Actions::Player::MOVE_RIGHT:
            {
                playerVector.x += speed;
                break;
            }
            case Actions::Player::JUMP:
            {
                break;
            }
        }

        this->accelerate(playerVector);
    }
}