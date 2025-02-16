#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "core/inc/SceneNode.hpp"
#include "core/inc/Messaging/MessageNetwork.hpp"

#include "application/inc/Player.hpp"

namespace Application
{
    enum Layer
    {
        Background = 0,
        Entity,
        LayerCount
    };


    class GameWorld
    {
        public:
            explicit GameWorld(sf::RenderWindow& window, Core::MessageNetwork* network);
    
            void update(sf::Time fixedTimeStep);
            void draw();
    
        private:
            void buildScene();
    
        private:
            sf::RenderWindow& mWindow;
            Core::MessageNetwork* mNetwork;
    
            Core::SceneNode mSceneGraph;
            std::array<Core::SceneNode*, LayerCount> mSceneLayers;

            Player* mPlayer;
    
    };
}
