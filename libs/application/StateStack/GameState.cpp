#include "application/StateStack/GameState.hpp"

#include <iostream>

Application::GameState::GameState( Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects ) :
    State( stack, stateIdentifier, sharedObjects ),
    mGameNetwork( *sharedObjects.network ),
    mScene( *sharedObjects.window, mGameNetwork, *sharedObjects.textures ),
    mKeyBindings(),
    mPlayerInputPublisher( mGameNetwork, mKeyBindings )
{
    std::cout << "Creating GameState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "Enter: Print message" << std::endl;
    std::cout << "P: Pause Game " << std::endl;
    std::cout << "Escape: Return To Menu State " << std::endl;
}

void Application::GameState::draw()
{
    mScene.draw();
}

bool Application::GameState::update( sf::Time fixedTimeStep )
{
    // Sends Message Notifications
    mGameNetwork.notifySubscribers();

    mScene.update( fixedTimeStep );

    return true;
}

bool Application::GameState::handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent )
{
    // Handle Event based Key Presses
    mPlayerInputPublisher.handleKeyPressed( keyPressedEvent );

    if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter )
    {
        std::cout << "Handling Events in GameState. You pressed the enter key " << std::endl;
    }
    else if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::P )
    {
        requestStackPush( States::Pause );
    }
    else if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape )
    {
        requestStackPop();
        requestStackPush( States::Menu );
    }

    return true;
}

bool Application::GameState::handleRealTimeInput()
{
    // Handle RealTime Input KeyPresses
    // Usually movement based
    mPlayerInputPublisher.handleRealTimeInput();

    return true;
}

Application::GameState::~GameState()
{
    // Any objects that persist over different States such as MessageNetwork, will need to
    // cleanup their resources
    mGameNetwork.shutdownNetwork();
}