#include "core/inc/State/State.hpp"
#include "core/inc/State/StateStack.hpp"

 Core::State::SharedObjects::SharedObjects(sf::RenderWindow& window) :
 window(&window)
 {
 }

 Core::State::State(StateStack& stack, std::string stateIdentifier, SharedObjects sObjects) :
 mStack(&stack),
 mStateIdentifierString(stateIdentifier),
 mSharedObjects(sObjects)
 {
 }

 Core::State::~State()
 {
 }

 void Core::State::requestStackPush(States::ID stateID)
 {
   mStack->pushState(stateID);
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
 

