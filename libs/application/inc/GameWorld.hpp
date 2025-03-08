#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "core/inc/SceneNode.hpp"
#include "core/inc/Messaging/MessageNetwork.hpp"

#include "application/inc/SceneGraph/PlayerNode.hpp"

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
        explicit GameWorld( sf::RenderWindow& window, Core::MessageNetwork& gameNetwork );

        void update( sf::Time fixedTimeStep );
        void draw();

      private:
        void buildSceneGraph();
        void correctPlayerVelocity();

      private:
        sf::RenderWindow& mWindow;
        Core::MessageNetwork& mGameNetwork;

        Core::SceneNode mSceneGraph;
        std::array< Core::SceneNode*, LayerCount > mSceneLayers;

        PlayerNode* mPlayer;
    };
}
