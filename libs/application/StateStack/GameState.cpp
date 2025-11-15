#include "application/StateStack/GameState.hpp"

#include "core/StateStack/StateStack.hpp"

#include <iostream>

std::string Application::GameState::identifier = "GameState";

Application::GameState::GameState( std::string stateIdentifier ) :
    State( stateIdentifier )
// mScene( *sharedObjects.window, mGameNetwork, *sharedObjects.textures ),
// mKeyBindings(),
// mPlayerInputPublisher( mGameNetwork, mKeyBindings )
{
    std::cout << "Creating GameState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "Enter: Print message" << std::endl;
    std::cout << "P: Pause Game " << std::endl;
    std::cout << "Escape: Return To Menu State " << std::endl;
}

Application::GameState::GameState() :
    State( "GameState " )
{}

// Used to set references to external subsystems
bool Application::GameState::initializeState()
{
    if ( !mStackRef )
        return false;

    mGameNetwork = mStackRef->getMessageNetworkRef();

    return true;
}

void Application::GameState::draw()
{
    // mScene.draw();
}

bool Application::GameState::update( sf::Time fixedTimeStep )
{
    // mScene.update( fixedTimeStep );

    return true;
}

bool Application::GameState::handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent )
{
    // Handle Event based Key Presses
    // mPlayerInputPublisher.handleKeyPressed( keyPressedEvent );

    if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter )
    {
        std::cout << "Handling Events in GameState. You pressed the enter key " << std::endl;
    }
    else if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::P )
    {
        requestStackPush( "PauseState" );
    }
    else if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape )
    {
        requestStackPop();
        requestStackPush( "MenuState" );
    }

    return true;
}

bool Application::GameState::handleRealTimeInput()
{
    // Handle RealTime Input KeyPresses
    // Usually movement based
    // mPlayerInputPublisher.handleRealTimeInput();

    return true;
}

Application::GameState::~GameState()
{
    // Any objects that persist over different States such as MessageNetwork, will need to
    // cleanup their resources
    // mGameNetwork.shutdownNetwork();
}