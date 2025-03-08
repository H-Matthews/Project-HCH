#include "application/inc/SceneGraph/SquareNode.hpp"

#include <iostream>

Application::SquareNode::SquareNode() :
    mRectangleShape( { 30.f, 30.f } )
{
    mRectangleShape.setFillColor( sf::Color( 250, 0, 0 ) );

    sf::FloatRect bounds = mRectangleShape.getLocalBounds();
    sf::Vector2f size = bounds.size;
    mRectangleShape.setOrigin( size );

    std::cout << mRectangleShape.getOrigin().x << " " << mRectangleShape.getOrigin().y << std::endl;
}

void Application::SquareNode::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mRectangleShape, states );
}