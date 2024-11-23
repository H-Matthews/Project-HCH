#include "core/inc/SceneNode.hpp"
#include "assert.h"

Core::SceneNode::SceneNode() : mParent(nullptr)
{
}

// We are attaching a child to a pre-existing SceneNode
// So, set the childs parent pointer to this 
// Add the child to this nodes children container
void Core::SceneNode::attachChild(std::unique_ptr<SceneNode> child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

std::unique_ptr<Core::SceneNode> Core::SceneNode::detachChild(const SceneNode& node)
{    
    auto found = std::find_if(mChildren.begin(), mChildren.end(), 
    [&] (std::unique_ptr<SceneNode>& p) -> bool {return p.get() == &node; });

    assert(found != mChildren.end());

    std::unique_ptr<SceneNode> result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);

    return result;
}

void Core::SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);
}

void Core::SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void Core::SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(const std::unique_ptr<SceneNode>& child : mChildren)
    {
        child->draw(target, states);
    }
}

void Core::SceneNode::update(sf::Time timeStep)
{
    updateCurrent(timeStep);
    updateChildren(timeStep);
}

void Core::SceneNode::updateCurrent(sf::Time)
{
}

void Core::SceneNode::updateChildren(sf::Time timeStep)
{
    for(const std::unique_ptr<SceneNode>& child : mChildren)
    {
        child->update(timeStep);
    }
}

// To compute Absolute Transform, we step up the class hierarchy and accumulate all relative 
// transforms at each stage until we reach the root. 
sf::Transform Core::SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for(const SceneNode* node = this;
        node != nullptr; node = node->mParent)
    {
        transform = node->getTransform() * transform;
    }

    return transform;
}

sf::Vector2f Core::SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}
