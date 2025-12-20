#include "core/StateStack/StateStack.hpp"

#include "core/StateStack/State.hpp"

#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include "core/Application.hpp"

#include <cassert>

const std::string Core::StateStack::TYPE_NAME = "StateStack";

Core::StateStack::~StateStack() = default;

Core::StateStack::StateStack( Application& application ) :
    Configurable( TYPE_NAME ),
    mStack(),
    mPendingRequests(),
    applicationRef( application )
{}

void Core::StateStack::update( sf::Time fixedTimeStep )
{
    // We should only update the relative state on the stack
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!( *itr )->update( fixedTimeStep ))
            break;
    }

    applyPendingChanges();
}

void Core::StateStack::draw()
{
    // We will always draw a state if its on the stack
    for (auto& state : mStack)
    {
        state->draw();
    }
}

void Core::StateStack::handleKeyPressed( const sf::Event::KeyPressed& keyPressed )
{
    // Depending on Event Type, Call different function
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!( *itr )->handleKeyPressed( keyPressed ))
            break;
    }
}

void Core::StateStack::handleMouseMoved( const sf::Event::MouseMoved& mouseMoved )
{
    // Depending on Event Type, Call different function
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!( *itr )->handleMouseMoved( mouseMoved ))
            break;
    }
}

void Core::StateStack::handleRealTimeInput()
{
    // Depending on Event Type, Call different function
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!( *itr )->handleRealTimeInput())
            break;
    }
}

void Core::StateStack::popState()
{
    mPendingRequests.push_back( PendingStateRequest( Action::POP ) );

    return;
}

void Core::StateStack::clearStates()
{
    mPendingRequests.push_back( PendingStateRequest( Action::CLEAR ) );
}

bool Core::StateStack::isEmpty() const
{
    return mStack.empty();
}

bool Core::StateStack::isPendingListEmpty() const
{
    return mPendingRequests.empty();
}

void Core::StateStack::initializeLogger()
{
    // Create and Register
    const std::string outDirectory = Utility::LogRegistry::instance()->getOutputDir();

    mLogger = Utility::createTextFileLogger(
        "StateStackLogger", outDirectory, "StateStack", ".log", Utility::LogLevel::INFO );
}

Core::MessageNetwork* Core::StateStack::getMessageNetworkRef()
{
    return applicationRef.getNetwork();
}

std::unique_ptr< Core::State > Core::StateStack::createState(
    const Core::StateStack::PendingStateRequest& changeRequest )
{
    auto createdState( changeRequest.stateConstructor() );
    createdState->setStackRef( this );
    createdState->initializeState();

    return createdState;
}

void Core::StateStack::applyPendingChanges()
{
    std::string logMessage;

    for (Core::StateStack::PendingStateRequest change : mPendingRequests)
    {
        logMessage.clear();
        logMessage += "State Transition --> ";
        switch (change.action)
        {
            case Action::PUSH:
            {
                mStack.push_back( createState( change ) );

                if constexpr (Utility::CAN_LOG)
                    logMessage += "PUSHING state: " + mStack.back()->getStateName();

                break;
            }
            case Action::POP:
            {
                if constexpr (Utility::CAN_LOG)
                    logMessage += "REMOVING state: " + mStack[ mStack.size() - 1 ]->getStateName();

                mStack.pop_back();
                break;
            }
            case Action::CLEAR:
            {
                if constexpr (Utility::CAN_LOG)
                    logMessage += "CLEARING all states on stack";

                mStack.clear();
                break;
            }
        }

        if constexpr (Utility::CAN_LOG)
        {
            mLogger->logInfo( logMessage );
        }
    }

    mPendingRequests.clear();
}

Core::StateStack::PendingStateRequest::PendingStateRequest( Action action ) :
    action( action ),
    stateConstructor()
{}