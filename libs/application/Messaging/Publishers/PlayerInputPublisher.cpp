#include "application/Messaging/Publishers/PlayerInputPublisher.hpp"

#include <cassert>

const int Application::PlayerInputPublisher::VECTOR_MESSAGE_SIZE = 10;

Application::PlayerInputPublisher::PlayerInputPublisher(Core::MessageNetwork& network,
                                                        Application::KeyBindings keyBindings)
    : Core::MessageNode(network, "PlayerInput Pub", Core::NodeType::PUBLISHER),
      mKeyBindings(keyBindings), mPlayerActionMessages(),
      mPlayerEventMessage(std::make_shared<PlayerInputMessage>(Messages::ID::PlayerActionMessage)) {
    // Add Topic to publish to
    addTopic(Messages::ID::PlayerActionMessage);

    // Allocate Memory up front so that we don't have to create messages during the game loop
    mPlayerActionMessages.reserve(VECTOR_MESSAGE_SIZE);
    for (int i = 0; i < VECTOR_MESSAGE_SIZE; ++i) {
        mPlayerActionMessages.emplace_back(
            std::make_shared<PlayerInputMessage>(Messages::ID::PlayerActionMessage));
    }
}

/*
    Handles KeyPressed events. Checks the keyBindings to see IF the current key is mapped to a
   Player Action IF its mapped to an action AND that action is not listed as a realtimeAction, then
   we send the message
*/
void Application::PlayerInputPublisher::handleKeyPressed(
    const sf::Event::KeyPressed& keyPressedEvent) {
    Actions::Player playerAction = mKeyBindings.getAssignedAction(keyPressedEvent.scancode);

    if (playerAction == Actions::Player::NONE || mKeyBindings.isRealTimeAction(playerAction))
        return;

    mPlayerEventMessage->action = playerAction;
    MessageNode::publish(mPlayerEventMessage);
}

/*
    This function is called at least once every frame. This removes the 250ms Operating System delay
   that is caused when trying to hold down a key. There is no delay when getting key presses with
   this function, so it needs to be used for gameplay level keys.

    NOTE: IF multiple keys are being selected at once, then the IF statement can be hit more than
   once in a single frame Thats why we allocate messages to a vector and reuse them.
*/
void Application::PlayerInputPublisher::handleRealTimeInput() {
    std::size_t keyCounter = 0;
    for (const auto& pair : mKeyBindings.getKeyBindings()) {
        if (sf::Keyboard::isKeyPressed(pair.first) && mKeyBindings.isRealTimeAction(pair.second)) {
            std::shared_ptr<PlayerInputMessage> message(getMessage(keyCounter++));

            message->action = pair.second;
            MessageNode::publish(message);
        }
    }
}

/*
    This function retrieves a message pointer. The index retrieves the next Message in the vector
   that we allocated This is required since the number of messages we send in one update loop is
   tied to how many keys we are currently pressing down Meaning, if we press down 10 keys at once,
   we will send 10 distinct messages in a single frame
*/
std::shared_ptr<Application::PlayerInputMessage>
Application::PlayerInputPublisher::getMessage(std::size_t index) {
    assert(index < mPlayerActionMessages.size());

    return mPlayerActionMessages[index];
}