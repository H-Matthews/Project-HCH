#include <gtest/gtest.h>

#include <vector>

// Unit(s) to be Tested
#include "core/inc/Messaging/MessageNetwork.hpp"
#include "core/inc/Messaging/MessageNode.hpp"

// BEGIN Helper Classes
enum class Action
{
    NONE = 0,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
};

class TestMessage : public Core::Message 
{
    public:
        TestMessage(const Messages::ID messageID );
        Action action;
};

TestMessage::TestMessage(const Messages::ID messageID ) : 
    Message( messageID, "PlayerActionMessage"),
    action(Action::NONE)
{
}

class TestPublisher : public Core::MessageNode
{
    public:
        TestPublisher(Core::MessageNetwork* messageNetwork);
        void update(Action action);

        std::unique_ptr<TestMessage> testMessage;
};

TestPublisher::TestPublisher(Core::MessageNetwork* messageNetwork) :
    Core::MessageNode(messageNetwork),
    testMessage()
{
    MessageNode::mMessageNodeInfo.nodeName = "PlayerActionPublisher";

    // Initialize Messages
    testMessage = std::make_unique<TestMessage>(Messages::ID::PlayerActionMessage);
}

void TestPublisher::update(Action action)
{
    switch(action)
    {
        case Action::MOVE_DOWN:
        {
            testMessage->action = Action::MOVE_DOWN;
            break;
        }
        case Action::MOVE_UP:
        {
            testMessage->action = Action::MOVE_UP;
            break;
        }
        case Action::MOVE_LEFT:
        {
            testMessage->action = Action::MOVE_LEFT;
            break;
        }
        case Action::MOVE_RIGHT:
        {
            testMessage->action = Action::MOVE_RIGHT;
            break;
        }
    }

    if(testMessage->action != Action::NONE)
        MessageNode::send(testMessage.get());
}

class TestSubscriberOne : public Core::MessageNode 
{
    private:
        std::vector<Action> messageVector;

    public:
        TestSubscriberOne(Core::MessageNetwork* messageNetwork);

        std::vector<Action> getMessageVector();

    private:
        void onNotify(Core::Message* message) override;
};

TestSubscriberOne::TestSubscriberOne(Core::MessageNetwork* messageNetwork) :
    MessageNode(messageNetwork, "TestComponentSubOne")
{
    MessageNode::subscribeTo(Messages::ID::PlayerActionMessage);
    MessageNode::registerSubscriberMessages();
}

std::vector<Action> TestSubscriberOne::getMessageVector()
{
    return messageVector;
}

void TestSubscriberOne::onNotify(Core::Message* message)
{
    TestMessage* playerActionMessage = dynamic_cast<TestMessage*>( message );
    if(playerActionMessage)
    {
        switch(playerActionMessage->action)
        {
            case Action::MOVE_DOWN:
            {
                messageVector.push_back(Action::MOVE_DOWN);
                break;
            }
            case Action::MOVE_UP:
            {
                messageVector.push_back(Action::MOVE_UP);
                break;
            }
            case Action::MOVE_LEFT:
            {
                messageVector.push_back(Action::MOVE_LEFT);
                break;
            }
            case Action::MOVE_RIGHT:
            {
                messageVector.push_back(Action::MOVE_RIGHT);\
                break;
            }
        }
    }
}

// END Helper Classes

// Test Fixture
class CoreMessageNetworkTest : public ::testing::Test 
{
    protected:
        Core::MessageNetwork testNetwork;
        std::unique_ptr< TestPublisher > publisher = nullptr;
        std::unique_ptr< TestSubscriberOne > subscriberOne = nullptr;
        
        void SetUp() override 
        {
            publisher = std::make_unique< TestPublisher >(&testNetwork);
            subscriberOne = std::make_unique< TestSubscriberOne >(&testNetwork);
        }
};

TEST_F(CoreMessageNetworkTest, ReceiveMessage)
{
    // Ensure messageVector is empty
    std::vector<Action> myActionMessages = subscriberOne->getMessageVector();

    ASSERT_EQ(myActionMessages.size(), 0);

    // Send a message
    Action upAction = Action::MOVE_UP;
    publisher->update(upAction);
    testNetwork.notifySubscribers();

    // Get Message vector again
    myActionMessages = subscriberOne->getMessageVector();

    // Ensure it has one message
    ASSERT_EQ(myActionMessages.size(), 1);
    
    // Ensure it has the upAction that we sent it
    ASSERT_EQ(myActionMessages[0], upAction);

}