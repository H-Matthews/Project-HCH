#pragma once

#include "application/inc/Scene/ECS/Systems/BaseSystem.hpp"

namespace System
{
    class RenderSystem : public BaseSystem
    {
      public:
        RenderSystem( sf::RenderWindow& window );

        void render() override;

      private:
        sf::RenderWindow& mWindow;
    };
}