#include "core/Application.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <iostream>

const sf::Time Core::Application::TIME_PER_FRAME = sf::seconds( 1.0f / 120.0f );
const std::string Core::Application::TYPE_NAME = "Application";

Core::Application::Application( Core::ConfigSpec configSpec ) :
    Configurable( TYPE_NAME ),
    mConfiguration( std::make_unique< Core::Configuration >( configSpec ) ),
    mState( State::NONE ),
    mTextures(),
    mNetwork(),
    mStateStack( *this ),
    mWindow( sf::VideoMode( { 640, 480 } ), "Application Window", sf::Style::Close )
{
    // Must call back up to the Configurable
    std::shared_ptr< ConfigNode > rootNode = ConfigurationTree::instance()->getRootNode();
    std::vector< std::shared_ptr< Core::ConfigNode > > childrenNodes = rootNode->getChildren();
    if ( childrenNodes.empty() )
        throw ConfigurationException( "Children were not populated for RootNode" );

    configure( rootNode );

    return;
}

Core::MessageNetwork* Core::Application::getNetwork()
{
    MessageNetwork* network( &mNetwork );
    return network;
}

void Core::Application::registerState(
    const std::string& stateIdentifier, std::function< std::unique_ptr< Core::State >() > registerFunc )
{
    mStateStack.registerState( stateIdentifier, registerFunc );

    return;
}

void Core::Application::pushState( const std::string& stateIdentifier )
{
    mStateStack.pushState( stateIdentifier );

    return;
}

void Core::Application::initialize()
{
    if ( !mConfiguration )
        throw ConfigurationException( "Configuration is NULL" );

    loadResources();

    if ( !mStateStack.isPendingListEmpty() )
        transitionState( State::WAITING_TO_RUN );

    return;
}

void Core::Application::run()
{
    if ( !transitionState( State::RUNNING ) )
    {
        std::string exceptionMessage =
            "Application is unable to transition RUNNING; Current State: " + convertAppStateEnumToString( mState );

        if constexpr ( Utility::CAN_LOG )
        {
            if ( mLogger )
                mLogger->logError( exceptionMessage );
        }

        throw ConfigurationException( exceptionMessage.c_str() );
    }

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    if constexpr ( Utility::CAN_LOG )
        mLogger->logInfo( "Entering main RUN loop" );

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
                    mLogger->logInfo( "Closing Window...." );
            }
        }

        render();
    }

    if constexpr ( Utility::CAN_LOG )
        mLogger->logInfo( "Exiting main RUN loop" );
}

void Core::Application::processInput()
{
    // SFMLs Window Class will detect events and then call these functions if the event matches
    // When needed, Add Event Subtypes here

    mWindow.handleEvents( [ this ]( const sf::Event::Closed& ) { mWindow.close(); },
        [ this ]( const sf::Event::KeyPressed& keyPressedEvent ) { mStateStack.handleKeyPressed( keyPressedEvent ); },
        [ this ]( const sf::Event::MouseMoved& mouseMovedEvent ) { mStateStack.handleMouseMoved( mouseMovedEvent ); } );

    mStateStack.handleRealTimeInput();

    mNetwork.notifySubscribers();
}

void Core::Application::update( sf::Time fixedTimeStep )
{
    mStateStack.update( fixedTimeStep );
}

void Core::Application::render()
{
    mWindow.clear( sf::Color::Cyan );

    mStateStack.draw();

    mWindow.setView( mWindow.getDefaultView() );
    mWindow.display();
}

void Core::Application::loadResources()
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

bool Core::Application::transitionState( State statusToTransfer )
{
    bool retStatus = false;

    if constexpr ( Utility::CAN_LOG )
        mLogger->logDebug( "Attempting to Transition to State: " + convertAppStateEnumToString( statusToTransfer ) );

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
        case State::WAITING_TO_RUN:
        {
            if ( mState == State::NONE )
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
            if ( mState == State::RUNNING )
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
            mLogger->logDebug( "Transitioning from State: " + convertAppStateEnumToString( prevState ) + " to " +
                               convertAppStateEnumToString( statusToTransfer ) );
    }
    else if ( Utility::CAN_LOG )
    {
        if constexpr ( Utility::CAN_LOG )
            mLogger->logError( "Could NOT transition from State: " + convertAppStateEnumToString( prevState ) + " to " +
                               convertAppStateEnumToString( statusToTransfer ) );
    }

    return retStatus;
}

std::string Core::Application::convertAppStateEnumToString( const State& state )
{
    std::string retString;

    switch ( state )
    {
        case State::NONE:
        {
            retString = "NONE";

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

Core::Application::State Core::Application::convertStringToAppStateEnum( const std::string& stringState )
{
    State retState = State::NONE;

    if ( stringState == "WAITING_TO_RUN" )
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