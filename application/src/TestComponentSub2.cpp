#include "TestComponentSub2.hpp"
#include "PlayerActionMessage.hpp"

TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    // Set Subscribe / Publish  topics here
    MessageNode::mSubscribeToTopics.push_back(Messages::ID::PlayerActionMessage);
    //MessageNode::mSubscribeToTopics.push_back(MessageTopic::ENEMY);
    //MessageNode::mSubscribeToTopics.push_back(MessageTopic::DROPS);

    registerSubscriberTopics();
}

void TestComponentSub2::onNotify(Message* message)
{
    PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>(message);

    if(pam != nullptr)
    {
        std::cout << "Receiver Node: " << MessageNode::mSubscriptionInfo.name << std::endl;
        std::cout << "Received Message: PlayerActionMessage " << std::endl;
        std::cout << "Sender: " << pam->getSenderName() << std::endl;
        if(pam->action == MOVE_UP)
        {
            std::cout << "Action: MOVE_UP " << std::endl;
        }
        else if(pam->action == MOVE_DOWN)
        {
            std::cout << "Action: MOVE_DOWN " << std::endl;
        }
    }
    std::cout << "\n";
}