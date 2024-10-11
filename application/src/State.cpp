#include "State.hpp"
#include "StateStack.hpp"

 State::SharedObjects::SharedObjects(sf::RenderWindow& window) :
 window(&window)
 {
 }

 State::State(StateStack& stack, std::string stateIdentifier, SharedObjects sObjects) :
 mStack(&stack),
 mStateIdentifierString(stateIdentifier),
 mSharedObjects(sObjects)
 {
 }

 State::~State()
 {
 }

 void State::requestStackPush(States::ID stateID)
 {
    mStack->pushState(stateID);
 }

 void State::requestStackPop()
 {
    mStack->popState();
 }

 void State::requestStateClear()
 {
    mStack->clearStates();
 }

 State::SharedObjects State::getSharedObjects() const
 {
    return mSharedObjects;
 }
 

