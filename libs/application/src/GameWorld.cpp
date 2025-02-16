#include "application/inc/GameWorld.hpp"


Application::GameWorld::GameWorld(sf::RenderWindow& window, Core::MessageNetwork* network) :
    mWindow(window),
    mNetwork(network),
    mSceneGraph(),
    mSceneLayers(),
    mPlayer(nullptr)
{
    buildScene();
}

void Application::GameWorld::update(sf::Time fixedTimeStep)
{
    // Update All Entities on SceneGraph
    mSceneGraph.update(fixedTimeStep);

    return;
}

void Application::GameWorld::draw()
{
    mWindow.draw(mSceneGraph);
}

void Application::GameWorld::buildScene()
{
    // Initialize Background & Entity SceneNode Layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        std::unique_ptr<Core::SceneNode> layer(new Core::SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Attach Player to Entity Layer
    std::unique_ptr<Player> mainPlayer(new Player(mNetwork));
    mPlayer = mainPlayer.get();
    mSceneLayers[Entity]->attachChild(std::move(mainPlayer));

}