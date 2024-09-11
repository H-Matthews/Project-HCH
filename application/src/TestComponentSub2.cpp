#include "TestComponentSub2.hpp"

TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    registerSubscriberMessages();
}

/* This class only handles the following Messages
1. PlayerActionMessage
*/
void TestComponentSub2::onNotify(Message*)
{
}