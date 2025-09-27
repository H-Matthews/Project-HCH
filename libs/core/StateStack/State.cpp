#include "core/StateStack/State.hpp"
#include "core/StateStack/StateStack.hpp"

Core::State::State( std::string stateIdentifier ) :
    mStack( nullptr ),
    mStateIdentifierString( stateIdentifier )
{}

Core::State::~State()
{}

const std::string Core::State::getStateName()
{
    return mStateIdentifierString;
}

void Core::State::requestStackPush( const std::string& stateIdentifier )
{
    mStack->pushState( stateIdentifier );
}

void Core::State::requestStackPop()
{
    mStack->popState();
}

void Core::State::requestStateClear()
{
    mStack->clearStates();
}
