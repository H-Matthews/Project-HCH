#pragma once

#include "core/inc/SceneNode.hpp"

namespace Application
{
    class EntityNode : public Core::SceneNode
    {
      public:
        void setVelocity( const sf::Vector2f velocity );
        void setVelocity( const float vx, const float vy );

        void accelerate( sf::Vector2f velocity );
        void accelerate( float vx, float vy );

        sf::Vector2f getVelocity() const;

      private:
        sf::Vector2f mVelocity;
        virtual void updateCurrent( sf::Time timeStep ) override;
    };
}