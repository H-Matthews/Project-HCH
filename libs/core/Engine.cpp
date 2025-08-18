#include "core/Engine.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/GameState.hpp"
#include "application/StateStack/PauseState.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <iostream>

const sf::Time Core::Engine::TIME_PER_FRAME = sf::seconds( 1.0f / 120.0f );
const std::string Core::Engine::TYPE_NAME = "Engine";

Core::Engine::Engine() :
    Configurable( TYPE_NAME ),
    mState( EngineState::NONE ),
    mEngineLogger( nullptr ),
    mTextures(),
    mConfiguration( nullptr ),
    mNetwork(),
    // mPlayerKeyBindings(),
    mWindow( sf::VideoMode( { 640, 480 } ), "Engine Window", sf::Style::Close ),
    mStateStack( Core::State::SharedObjects( mWindow, mNetwork, mTextures ) )
{
    if constexpr ( Utility::CAN_LOG )
        mEngineLogger = Configurable::createLogger();
}

void Core::Engine::initialize()
{
    if ( !mConfiguration )
        throw ConfigurationException( "Configuration is NULL" );

    // Create CORE CONFIGURABLES ----------------------------
    // auto gameNetwork =
    //     Core::ConfigurableFactory::createTypedConfigurable< Core::MessageNetwork >( Core::MessageNetwork::TYPE_NAME
    //     );

    // if constexpr ( Utility::CAN_LOG )
    //     gameNetwork->initializeLogger();

    loadResources();

    registerStates();
    mStateStack.pushState( States::Menu );

    if ( !mStateStack.isPendingListEmpty() )
        transitionState( EngineState::WAITING_TO_RUN );

    return;
}

void Core::Engine::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    if ( mState == EngineState::NONE )
    {
        // Throw Configuration Exception
        return;
    }

    if ( !transitionState( EngineState::RUNNING ) )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            if ( mEngineLogger )
            {
                mEngineLogger->logError(
                    "Application is unable to transition to the RUNNING state... Likely a Configuration Error " );
            }
        }

        return;
    }

    if constexpr ( Utility::CAN_LOG )
        mEngineLogger->logInfo( "Entering main RUN loop" );

    while ( mWindow.isOpen() )
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while ( timeSinceLastUpdate > TIME_PER_FRAME )
        {
            timeSinceLastUpdate -= TIME_PER_FRAME;

            processInput();
            update( TIME_PER_FRAME );

            if ( mStateStack.isEmpty() )
            {
                mWindow.close();

                if constexpr ( Utility::CAN_LOG )
                    mEngineLogger->logInfo( "Closing Window...." );
            }
        }

        render();
    }

    if constexpr ( Utility::CAN_LOG )
        mEngineLogger->logInfo( "Exiting main RUN loop" );
}

void Core::Engine::processInput()
{
    // SFMLs Window Class will detect events and then call these functions if the event matches
    // When needed, Add Event Subtypes here

    mWindow.handleEvents( [ this ]( const sf::Event::Closed& ) { mWindow.close(); },
        [ this ]( const sf::Event::KeyPressed& keyPressedEvent ) { mStateStack.handleKeyPressed( keyPressedEvent ); },
        [ this ]( const sf::Event::MouseMoved& mouseMovedEvent ) { mStateStack.handleMouseMoved( mouseMovedEvent ); } );

    mStateStack.handleRealTimeInput();
}

void Core::Engine::update( sf::Time fixedTimeStep )
{
    mStateStack.update( fixedTimeStep );
}

void Core::Engine::render()
{
    mWindow.clear( sf::Color::Cyan );

    mStateStack.draw();

    mWindow.setView( mWindow.getDefaultView() );
    mWindow.display();
}

// TODO: This register call should take a string to a State Identifier
// that way we can inject states from the game application library
void Core::Engine::registerStates()
{
    // mStateStack.registerState< Application::MenuState >( States::Menu );
    // mStateStack.registerState< Application::GameState >( States::Game );
    // mStateStack.registerState< Application::PauseState >( States::Pause );
}

void Core::Engine::loadResources()
{
    // auto fontTexturePaths = mConfiguration->getAssetPaths();
    // std::string texturePath = fontTexturePaths.first;
    // std::string fontPath = fontTexturePaths.second;

    // TODO: This needs to be reexamined. Shouldn't hardcode file names like this
    // Should tie this to the config file...
    // mTextures.load( Textures::ID::PLAYER, texturePath + "/" + "playerSprite.png" );
    // mTextures.load( Textures::ID::BACKGROUND, texturePath + "/" + "background.png" );
    // mTextures.load( Textures::ID::ENEMY, texturePath + "/" + "enemySprite.png" );

    return;
}

bool Core::Engine::transitionState( EngineState statusToTransfer )
{
    bool retStatus = false;

    if constexpr ( Utility::CAN_LOG )
        mEngineLogger->logDebug(
            "Attempting to Transition to State: " + convertEngineStateEnumToString( statusToTransfer ) );

    if ( mState == statusToTransfer )
        return retStatus;

    EngineState prevState = EngineState::NONE;

    switch ( statusToTransfer )
    {
        case EngineState::NONE:
        {
            // DO NOTHING

            break;
        }
        case EngineState::WAITING_TO_RUN:
        {
            if ( mState == EngineState::NONE )
            {
                prevState = mState;

                mState = statusToTransfer;
                retStatus = true;
            }

            break;
        }
        case EngineState::RUNNING:
        {
            if ( mState == EngineState::WAITING_TO_RUN )
            {
                prevState = mState;

                mState = statusToTransfer;
                retStatus = true;
            }

            break;
        }
        case EngineState::SHUTTING_DOWN:
        {
            if ( mState == EngineState::WAITING_TO_RUN )
            {
                prevState = mState;

                mState = statusToTransfer;
                retStatus = true;
            }

            break;
        }
    }

    if ( Utility::CAN_LOG && retStatus )
    {
        if constexpr ( Utility::CAN_LOG )
            mEngineLogger->logDebug( "Transitioning from State: " + convertEngineStateEnumToString( prevState ) +
                                     " to " + convertEngineStateEnumToString( statusToTransfer ) );
    }
    else if ( Utility::CAN_LOG )
    {
        if constexpr ( Utility::CAN_LOG )
            mEngineLogger->logError( "Could NOT transition from State: " + convertEngineStateEnumToString( prevState ) +
                                     " to " + convertEngineStateEnumToString( statusToTransfer ) );
    }

    return retStatus;
}

std::string Core::convertEngineStateEnumToString( const EngineState& state )
{
    std::string retString;

    switch ( state )
    {
        case EngineState::NONE:
        {
            retString = "NONE";

            break;
        }
        case EngineState::WAITING_TO_RUN:
        {
            retString = "WAITING_TO_RUN";

            break;
        }
        case EngineState::RUNNING:
        {
            retString = "RUNNING";

            break;
        }
        case EngineState::SHUTTING_DOWN:
        {
            retString = "SHUTTING_DOWN";

            break;
        }
    }

    return retString;
}
Core::EngineState Core::convertStringToEngineStateEnum( const std::string& stringState )
{
    EngineState retState = EngineState::NONE;

    if ( stringState == "WAITING_TO_RUN" )
    {
        retState = EngineState::WAITING_TO_RUN;
    }
    else if ( stringState == "RUNNING" )
    {
        retState = EngineState::RUNNING;
    }
    else if ( stringState == "SHUTTING_DOWN" )
    {
        retState = EngineState::SHUTTING_DOWN;
    }

    return retState;
}