#pragma once

#include <SFML/Graphics.hpp>

#include "application/inc/SceneGraph/EntityNode.hpp"

namespace Application
{
    class SquareNode : public Application::EntityNode
    {
        public:
            SquareNode();

            // Derives from SceneNode
            void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

        private:
            sf::RectangleShape mRectangleShape;
    };

}

