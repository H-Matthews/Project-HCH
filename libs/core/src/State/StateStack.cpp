#include "core/inc/State/StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::SharedObjects sObjects) : 
mStack(),
mPendingList(),
mSharedObjects(sObjects),
mRegistry()
{
}

void StateStack::update(sf::Time fixedTimeStep)
{
    // We should only update the relative state on the stack
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update(fixedTimeStep) )
            break;
    }

    applyPendingChanges();
}

void StateStack::draw()
{
    // We will always draw a state if its on the stack
    for(std::unique_ptr<State>& state : mStack)
    {
        state->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event)
{
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if(!(*itr)->handleEvent(event))
            break;
    }

    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(pendingStateRequests(Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(pendingStateRequests(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(pendingStateRequests(Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

std::unique_ptr<State> StateStack::createState(States::ID stateID)
{
    auto found = mRegistry.find(stateID);
    assert(found != mRegistry.end());

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for( StateStack::pendingStateRequests change: mPendingList)
    {
        switch(change.action)
        {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;

            case Pop:
                mStack.pop_back();
                break;

            case Clear:
                mStack.clear();
                break;
        }
    }

    mPendingList.clear();
}

StateStack::pendingStateRequests::pendingStateRequests(Action action, States::ID stateID) : 
action(action),
stateID(stateID)
{
}