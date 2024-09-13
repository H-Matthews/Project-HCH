#include "TestComponentSub.hpp"
#include "PlayerActionMessage.hpp"

#include <iostream>

TestComponentSub::TestComponentSub(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub")
{
    MessageNode::subscribeTo(Message::ID::PlayerActionMessage);
    registerSubscriberMessages();
}

void TestComponentSub::onNotify(Message* message)
{
    std::cout << MessageNode::mMessageNodeInfo.nodeName << " recieved " << message->getStringMessageID() << std::endl;

    PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>( message );
    if(pam)
    {
        if(pam->action == Action::MOVE_DOWN)
            std::cout << "Moving Down " << std::endl;
    }

    std::cout << "\n";
}