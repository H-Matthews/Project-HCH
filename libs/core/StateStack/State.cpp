#include "core/StateStack/State.hpp"
#include "core/StateStack/StateStack.hpp"

Core::State::SharedObjects::SharedObjects() :
    window( nullptr ),
    network( nullptr ),
    textures( nullptr )
{}

Core::State::SharedObjects::SharedObjects(
    sf::RenderWindow& window, Core::MessageNetwork& network, TextureHolder& textures ) :
    window( &window ),
    network( &network ),
    textures( &textures )
{}

Core::State::State( StateStack& stack, std::string stateIdentifier, SharedObjects sObjects ) :
    mStack( &stack ),
    mStateIdentifierString( stateIdentifier ),
    mSharedObjects( sObjects )
{}

Core::State::~State()
{}

void Core::State::requestStackPush( States::ID stateID )
{
    mStack->pushState( stateID );
}

void Core::State::requestStackPop()
{
    mStack->popState();
}

void Core::State::requestStateClear()
{
    mStack->clearStates();
}

Core::State::SharedObjects Core::State::getSharedObjects() const
{
    return mSharedObjects;
}

const std::string Core::State::getStateAsString()
{
    return mStateIdentifierString;
}
