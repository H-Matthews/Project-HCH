#include "core/StateStack/State.hpp"
#include "core/StateStack/StateStack.hpp"

Core::State::State( std::string stateIdentifier ) :
    mStackRef( nullptr ),
    mStateIdentifierString( stateIdentifier )
{}

Core::State::~State()
{}

const std::string Core::State::getStateName()
{
    return mStateIdentifierString;
}

void Core::State::setStackRef( StateStack* stack )
{
    mStackRef = stack;

    return;
}

void Core::State::requestStackPush( const std::string& stateIdentifier )
{
    mStackRef->pushState( stateIdentifier );
}

void Core::State::requestStackPop()
{
    mStackRef->popState();
}

void Core::State::requestStateClear()
{
    mStackRef->clearStates();
}
