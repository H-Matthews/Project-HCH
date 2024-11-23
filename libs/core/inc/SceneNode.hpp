#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Core
{
    class SceneNode : public sf::Transformable, public sf::Drawable
    {
        public:
            SceneNode();
            void update(sf::Time fixedTimeStep);

            sf::Transform getWorldTransform() const;
            sf::Vector2f getWorldPosition() const;

            void attachChild(std::unique_ptr<SceneNode> child);
            std::unique_ptr<SceneNode> detachChild(const SceneNode& node);

        private:
            std::vector<std::unique_ptr<SceneNode>> mChildren;
            SceneNode* mParent;

        private:
            virtual void updateCurrent(sf::Time fixedTimeStep);
            void updateChildren(sf::Time fixedTimeStep);

            // Draw is derived from Drawable and is called from SFMLs Window
            // That is why its listed as private
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
            virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
            void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}