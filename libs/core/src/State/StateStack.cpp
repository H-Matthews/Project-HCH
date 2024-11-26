#include "core/inc/State/StateStack.hpp"

#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <cassert>

Core::StateStack::StateStack(Core::State::SharedObjects sObjects) : 
mLogger(nullptr),
mStack(),
mPendingList(),
mSharedObjects(sObjects),
mRegistry()
{
}

void Core::StateStack::update(sf::Time fixedTimeStep)
{
    // We should only update the relative state on the stack
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update(fixedTimeStep) )
            break;
    }

    applyPendingChanges();
}

void Core::StateStack::draw()
{
    // We will always draw a state if its on the stack
    for(std::unique_ptr<Core::State>& state : mStack)
    {
        state->draw();
    }
}

void Core::StateStack::handleEvent(const sf::Event& event)
{
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if(!(*itr)->handleEvent(event))
            break;
    }

    applyPendingChanges();
}

void Core::StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(pendingStateRequests(Push, stateID));
}

void Core::StateStack::popState()
{
    mPendingList.push_back(pendingStateRequests(Pop));
}

void Core::StateStack::clearStates()
{
    mPendingList.push_back(pendingStateRequests(Clear));
}

bool Core::StateStack::isEmpty() const
{
    return mStack.empty();
}

void Core::StateStack::initializeLogger()
{
    // Create and Register
    const std::string outDirectory = Utility::LogRegistry::instance()->getAppOutputDir();

    mLogger = Utility::Factory::createTextFileLogger("StateStackLogger", outDirectory, "StateStack", 
                                                     ".log", Utility::LogLevel::INFO);
}

std::unique_ptr<Core::State> Core::StateStack::createState(States::ID stateID)
{
    auto found = mRegistry.find(stateID);
    assert(found != mRegistry.end());

    return found->second();
}

void Core::StateStack::applyPendingChanges()
{
    std::stringstream logStream;

    for( Core::StateStack::pendingStateRequests change: mPendingList)
    {
        logStream.str("");
        logStream.clear();

        logStream << "State Transition --> ";
        switch(change.action)
        {
            case Push:
                logStream << "Pushing State: " << States::statesEnumToString(change.stateID);
                mLogger->logInfo(logStream.str());

                mStack.push_back(createState(change.stateID));
                break;

            case Pop:
                logStream << "Removing State: " << mStack[mStack.size() - 1]->getStateAsString();
                mLogger->logInfo(logStream.str());

                mStack.pop_back();
                break;

            case Clear:
                logStream << "Clearing the Stack";
                mLogger->logInfo(logStream.str());
                
                mStack.clear();
                break;
        }
    }

    mPendingList.clear();
}

Core::StateStack::pendingStateRequests::pendingStateRequests(Action action, States::ID stateID) : 
action(action),
stateID(stateID)
{
}