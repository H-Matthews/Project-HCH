#include "application/App.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/GameState.hpp"
#include "application/StateStack/PauseState.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <iostream>

const sf::Time Application::App::TIME_PER_FRAME = sf::seconds( 1.0f / 120.0f );

Application::App::App( std::unique_ptr< Core::ConfigurationI > config ) :
    mState( State::NOT_CONFIGURED ),
    mAppLogger( std::make_shared< Utility::Logger >( "AppLogger" ) ),
    mTextures(),
    mConfiguration( std::move( config ) ),
    mNetwork(),
    mPlayerKeyBindings(),
    mWindow( sf::VideoMode( { 640, 480 } ), "App Window", sf::Style::Close ),
    mStateStack( Core::State::SharedObjects( mWindow, mNetwork, mTextures ) )
{}

void Application::App::initialize()
{
    // 1. INITIALIZE CONFIGURATION
    mConfiguration->initializeOutputDirectory();
    mConfiguration->initializeConfigDirectory();
    mConfiguration->initializeAssetsDirectory();

    bool retStatus = mConfiguration->parse();

    // 2. INITIALIZE APP, CORE LOGGERS
    if constexpr ( Utility::CAN_LOG )
    {
        initializeAppLogger();
        initializeCoreLoggers();
    }

    if ( retStatus )
        transitionState( State::CONFIGURED );

    // 3. LOAD ASSETS
    loadResources();

    // 4. INITIALIZE STATE STACK
    registerStates();
    mStateStack.pushState( States::Menu );

    if ( !mStateStack.isPendingListEmpty() )
        transitionState( State::WAITING_TO_RUN );

    return;
}

void Application::App::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    if ( !transitionState( State::RUNNING ) )
    {
        if constexpr ( Utility::CAN_LOG )
            mAppLogger->logError(
                "Application is unable to transition to the RUNNING state... Likely a Configuration Error " );

        return;
    }

    if constexpr ( Utility::CAN_LOG )
        mAppLogger->logInfo( "Entering main RUN loop" );

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
                    mAppLogger->logInfo( "Closing Window...." );
            }
        }

        render();
    }

    if constexpr ( Utility::CAN_LOG )
        mAppLogger->logInfo( "Exiting main RUN loop" );
}

void Application::App::processInput()
{
    // SFMLs Window Class will detect events and then call these functions if the event matches
    // When needed, Add Event Subtypes here

    mWindow.handleEvents( [ this ]( const sf::Event::Closed& ) { mWindow.close(); },
        [ this ]( const sf::Event::KeyPressed& keyPressedEvent ) { mStateStack.handleKeyPressed( keyPressedEvent ); },
        [ this ]( const sf::Event::MouseMoved& mouseMovedEvent ) { mStateStack.handleMouseMoved( mouseMovedEvent ); } );

    mStateStack.handleRealTimeInput();
}

void Application::App::update( sf::Time fixedTimeStep )
{
    mStateStack.update( fixedTimeStep );
}

void Application::App::render()
{
    mWindow.clear( sf::Color::Cyan );

    mStateStack.draw();

    mWindow.setView( mWindow.getDefaultView() );
    mWindow.display();
}

void Application::App::initializeAppLogger()
{
    // Configure App Logger
    const std::string appOutputDir = Utility::LogRegistry::instance()->getAppOutputDir();

    // Setup Sinks
    auto textFileSink =
        std::make_shared< Utility::TextFileSink >( appOutputDir, "App", ".log", Utility::LogLevel::DEBUG );

    auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >( Utility::LogLevel::INFO );

    // Add Sinks to Logger
    Utility::Logger::sinkList list = { colorConsoleSink, textFileSink };
    mAppLogger->addSinkList( list );

    // Register App Logger
    Utility::LogRegistry::instance()->registerLogger( mAppLogger );
}

void Application::App::initializeCoreLoggers()
{
    mStateStack.initializeLogger();
    mNetwork.initializeLogger();
}

void Application::App::registerStates()
{
    mStateStack.registerState< Application::MenuState >( States::Menu );
    mStateStack.registerState< Application::GameState >( States::Game );
    mStateStack.registerState< Application::PauseState >( States::Pause );
}

void Application::App::loadResources()
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

bool Application::App::transitionState( State statusToTransfer )
{
    bool retStatus = false;

    if constexpr ( Utility::CAN_LOG )
        mAppLogger->logDebug( "Attempting to Transition to State: " + convertEnumToString( statusToTransfer ) );

    if ( mState == statusToTransfer )
        return retStatus;

    State prevState = State::NONE;

    switch ( statusToTransfer )
    {
        case State::NONE:
        {
            // DO NOTHING

            break;
        }
        case State::NOT_CONFIGURED:
        {
            // DO NOTHING

            break;
        }
        case State::CONFIGURED:
        {
            if ( mState == State::NOT_CONFIGURED )
            {
                prevState = mState;

                mState = statusToTransfer;
                retStatus = true;
            }

            break;
        }
        case State::WAITING_TO_RUN:
        {
            if ( mState == State::CONFIGURED )
            {
                prevState = mState;

                mState = statusToTransfer;
                retStatus = true;
            }

            break;
        }
        case State::RUNNING:
        {
            if ( mState == State::WAITING_TO_RUN )
            {
                prevState = mState;

                mState = statusToTransfer;
                retStatus = true;
            }

            break;
        }
        case State::SHUTTING_DOWN:
        {
            if ( mState == State::WAITING_TO_RUN )
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
            mAppLogger->logDebug( "Transitioning from State: " + convertEnumToString( prevState ) + " to " +
                                  convertEnumToString( statusToTransfer ) );
    }
    else if ( Utility::CAN_LOG )
    {
        if constexpr ( Utility::CAN_LOG )
            mAppLogger->logError( "Could NOT transition from State: " + convertEnumToString( prevState ) + " to " +
                                  convertEnumToString( statusToTransfer ) );
    }

    return retStatus;
}

std::string Application::convertEnumToString( const State& state )
{
    std::string retString;

    switch ( state )
    {
        case State::NONE:
        {
            retString = "NONE";

            break;
        }
        case State::NOT_CONFIGURED:
        {
            retString = "NOT_CONFIGURED";

            break;
        }
        case State::CONFIGURED:
        {
            retString = "CONFIGURED";

            break;
        }
        case State::WAITING_TO_RUN:
        {
            retString = "WAITING_TO_RUN";

            break;
        }
        case State::RUNNING:
        {
            retString = "RUNNING";

            break;
        }
        case State::SHUTTING_DOWN:
        {
            retString = "SHUTTING_DOWN";

            break;
        }
    }

    return retString;
}
Application::State Application::convertStringToEnum( const std::string& stringState )
{
    State retState = State::NONE;

    if ( stringState == "NOT_CONFIGURED" )
    {
        retState = State::NOT_CONFIGURED;
    }
    else if ( stringState == "CONFIGURED" )
    {
        retState = State::CONFIGURED;
    }
    else if ( stringState == "WAITING_TO_RUN" )
    {
        retState = State::WAITING_TO_RUN;
    }
    else if ( stringState == "RUNNING" )
    {
        retState = State::RUNNING;
    }
    else if ( stringState == "SHUTTING_DOWN" )
    {
        retState = State::SHUTTING_DOWN;
    }

    return retState;
}