#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork, "TestComponentPublish")
{
    // Set Subscribe / Publish  topics here
    MessageNode::mPublishToTopics.push_back(Messages::ID::PlayerActionMessage);
    //MessageNode::mPublishToTopics.push_back(MessageTopic::ENEMY);
    //MessageNode::mPublishToTopics.push_back(MessageTopic::DROPS);

    mPublisherName = mSubscriptionInfo.name;
}

void TestComponentPub::update()
{
    //auto msg = std::make_unique< PlayerActionMessage > ();

    auto msg = createMessage(Messages::ID::PlayerActionMessage);

    // You can dynamically add more mPublishToTopics if needed
    // MessageNode::mPublishToTopics.push_back(MessageTopic::DROPS);

    send(msg.get());
}