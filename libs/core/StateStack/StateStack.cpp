#include "core/StateStack/StateStack.hpp"

#include "core/StateStack/State.hpp"

#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <cassert>

const std::string Core::StateStack::TYPE_NAME = "StateStack";

Core::StateStack::StateStack( Application& application ) :
    Configurable( TYPE_NAME ),
    mStack(),
    mPendingRequests(),
    mRegistry(),
    applicationRef( application )
{}

void Core::StateStack::update( sf::Time fixedTimeStep )
{
    // We should only update the relative state on the stack
    for ( auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr )
    {
        if ( !( *itr )->update( fixedTimeStep ) )
            break;
    }

    applyPendingChanges();
}

void Core::StateStack::draw()
{
    // We will always draw a state if its on the stack
    for ( auto& state : mStack )
    {
        state->draw();
    }
}

void Core::StateStack::handleKeyPressed( const sf::Event::KeyPressed& keyPressed )
{
    // Depending on Event Type, Call different function
    for ( auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr )
    {
        if ( !( *itr )->handleKeyPressed( keyPressed ) )
            break;
    }
}

void Core::StateStack::handleMouseMoved( const sf::Event::MouseMoved& mouseMoved )
{
    // Depending on Event Type, Call different function
    for ( auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr )
    {
        if ( !( *itr )->handleMouseMoved( mouseMoved ) )
            break;
    }
}

void Core::StateStack::handleRealTimeInput()
{
    // Depending on Event Type, Call different function
    for ( auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr )
    {
        if ( !( *itr )->handleRealTimeInput() )
            break;
    }
}

void Core::StateStack::registerState(
    const std::string& stateIdentifier, std::function< std::unique_ptr< Core::State >() > registerFunc )
{
    mRegistry.insert( { stateIdentifier, registerFunc } );

    return;
}

void Core::StateStack::pushState( const std::string& stateIdentifier )
{
    mPendingRequests.push_back( PendingStateRequest( Push, stateIdentifier ) );

    return;
}

void Core::StateStack::popState()
{
    mPendingRequests.push_back( PendingStateRequest( Pop ) );
}

void Core::StateStack::clearStates()
{
    mPendingRequests.push_back( PendingStateRequest( Clear ) );
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

std::unique_ptr< Core::State > Core::StateStack::createState( std::string stateIdentifier )
{
    auto found = mRegistry.find( stateIdentifier );
    assert( found != mRegistry.end() );

    std::unique_ptr< Core::State > createdState( found->second() );
    createdState->setStackRef( this );

    return createdState;
}

void Core::StateStack::applyPendingChanges()
{
    std::string logMessage;

    for ( Core::StateStack::PendingStateRequest change : mPendingRequests )
    {
        logMessage.clear();
        logMessage += "State Transition --> ";
        switch ( change.action )
        {
            case Push:
            {
                if constexpr ( Utility::CAN_LOG )
                {
                    logMessage += "Pushing State: " + change.stateIdentifier;
                    mLogger->logInfo( logMessage );
                }

                mStack.push_back( createState( change.stateIdentifier ) );
                break;
            }
            case Pop:
            {
                if constexpr ( Utility::CAN_LOG )
                {
                    logMessage += "Removing State: " + mStack[ mStack.size() - 1 ]->getStateName();
                    mLogger->logInfo( logMessage );
                }

                mStack.pop_back();
                break;
            }
            case Clear:
            {
                if constexpr ( Utility::CAN_LOG )
                {
                    logMessage += "Clearing the Stack";
                    mLogger->logInfo( logMessage );
                }

                mStack.clear();
                break;
            }
        }
    }

    mPendingRequests.clear();
}

Core::StateStack::PendingStateRequest::PendingStateRequest( Action action, const std::string& stateIdentifier ) :
    action( action ),
    stateIdentifier( stateIdentifier )
{}