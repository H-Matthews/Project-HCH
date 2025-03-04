#include "application/inc/GameWorld.hpp"
#include "application/inc/SceneGraph/SquareNode.hpp"

#include <cmath>

Application::GameWorld::GameWorld(sf::RenderWindow& window, Core::MessageNetwork& gameNetwork) :
    mWindow(window),
    mGameNetwork(gameNetwork),
    mSceneGraph(),
    mSceneLayers(),
    mPlayer(nullptr)
{
    buildSceneGraph();
}

void Application::GameWorld::update(sf::Time fixedTimeStep)
{
    // Set Velocity back to 0 after updating objects position
    mPlayer->setVelocity(0.f, 0.f);

    // Disseminate Messages
    mGameNetwork.notifySubscribers();

    // Determines whether or not we changed our X and Y velocity values in the same update cycle
    // IF so, we must correct our velocity values because we will be going too fast
    correctPlayerVelocity();

    // Update Entities on SceneGraph
    mSceneGraph.update(fixedTimeStep);

    return;
}

void Application::GameWorld::draw()
{
    mWindow.draw(mSceneGraph);

    return;
}

void Application::GameWorld::buildSceneGraph()
{
    // Initialize Background & Entity SceneNode Layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        std::unique_ptr<Core::SceneNode> layer(new Core::SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Attach Player to Entity Layer
    std::unique_ptr<PlayerNode> mainPlayer(new PlayerNode(mGameNetwork));
    mPlayer = mainPlayer.get();
    mPlayer->setPosition( {320.f, 240.f} );
    mSceneLayers[Entity]->attachChild(std::move(mainPlayer));

    // Attach two more Shapes to the mainPlayer to demonstrate the Scene Graph
    std::unique_ptr<SquareNode> rightSquare(new SquareNode());
    rightSquare->setPosition( {40.f, 20.f} );
    mPlayer->attachChild(std::move(rightSquare));

    std::unique_ptr<SquareNode> leftSquare(new SquareNode());
    leftSquare->setPosition( {-70.f, 20.f} );
    mPlayer->attachChild(std::move(leftSquare));

    return;
}

void Application::GameWorld::correctPlayerVelocity()
{
    sf::Vector2f velocity = mPlayer->getVelocity();

    // IF we modified both velocitys in the same cycle, then divide velocity by sqrt(2)
    // This happens when the user presses two movement keys at the same time
    if(velocity.x != 0.f && velocity.y != 0.f)
        mPlayer->setVelocity(velocity / std::sqrt(2.f));

    return;
}