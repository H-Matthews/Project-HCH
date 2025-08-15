#include "core/StateStack/StateStack.hpp"

#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <cassert>

const std::string Core::StateStack::TYPE_NAME = "STATE_STACK";

Core::StateStack::StateStack() :
    Configurable( TYPE_NAME ),
    mLogger( nullptr ),
    mStack(),
    mPendingList(),
    mSharedObjects(),
    mRegistry()
{}

Core::StateStack::StateStack( Core::State::SharedObjects sObjects ) :
    Configurable( TYPE_NAME ),
    mLogger( nullptr ),
    mStack(),
    mPendingList(),
    mSharedObjects( sObjects ),
    mRegistry()
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
    for ( std::unique_ptr< Core::State >& state : mStack )
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

void Core::StateStack::pushState( States::ID stateID )
{
    mPendingList.push_back( pendingStateRequests( Push, stateID ) );
}

void Core::StateStack::popState()
{
    mPendingList.push_back( pendingStateRequests( Pop ) );
}

void Core::StateStack::clearStates()
{
    mPendingList.push_back( pendingStateRequests( Clear ) );
}

bool Core::StateStack::isEmpty() const
{
    return mStack.empty();
}

bool Core::StateStack::isPendingListEmpty() const
{
    return mPendingList.empty();
}

void Core::StateStack::initializeLogger()
{
    // Create and Register
    const std::string outDirectory = Utility::LogRegistry::instance()->getAppOutputDir();

    mLogger = Utility::createTextFileLogger(
        "StateStackLogger", outDirectory, "StateStack", ".log", Utility::LogLevel::INFO );
}

std::unique_ptr< Core::State > Core::StateStack::createState( States::ID stateID )
{
    auto found = mRegistry.find( stateID );
    assert( found != mRegistry.end() );

    return found->second();
}

void Core::StateStack::applyPendingChanges()
{
    std::string logMessage;

    for ( Core::StateStack::pendingStateRequests change : mPendingList )
    {
        logMessage.clear();
        logMessage += "State Transition --> ";
        switch ( change.action )
        {
            case Push:
                if constexpr ( Utility::CAN_LOG )
                {
                    logMessage += "Pushing State: " + States::statesEnumToString( change.stateID );
                    mLogger->logInfo( logMessage );
                }

                mStack.push_back( createState( change.stateID ) );
                break;

            case Pop:
                if constexpr ( Utility::CAN_LOG )
                {
                    logMessage += "Removing State: " + mStack[ mStack.size() - 1 ]->getStateAsString();
                    mLogger->logInfo( logMessage );
                }

                mStack.pop_back();
                break;

            case Clear:
                if constexpr ( Utility::CAN_LOG )
                {
                    logMessage += "Clearing the Stack";
                    mLogger->logInfo( logMessage );
                }

                mStack.clear();
                break;
        }
    }

    mPendingList.clear();
}

Core::StateStack::pendingStateRequests::pendingStateRequests( Action action, States::ID stateID ) :
    action( action ),
    stateID( stateID )
{}