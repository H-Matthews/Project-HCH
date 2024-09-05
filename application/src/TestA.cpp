#include "TestA.hpp"


TestA::TestA(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork)
{
    MessageFlag subscribeToTopics(0);
    subscribeToTopics.setFlag(MessageTopics::PLAYER);
    subscribeToTopics.setFlag(MessageTopics::ENEMY);

    mSubscribeTopics = subscribeToTopics;

    MessageNode::registerMessageListener();
}

void TestA::onNotify(Message message)
{
    if(message.mMessageRecipients.hasFlag(MessageTopics::PLAYER))
    {
        std::cout << "This is for a Player Node " << std::endl;
    }

    if(message.mMessageRecipients.hasFlag(MessageTopics::ENEMY))
    {
        std::cout << "This is for an enemy Node " << std::endl;
    }

    if(message.mMessageType == MessageType::Movement)
    {
        std::cout << "Movement Message" << std::endl;
    }
}