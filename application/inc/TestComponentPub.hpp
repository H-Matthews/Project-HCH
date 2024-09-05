#include "MessageNode.hpp"

#include <iostream>

// This class will be the Subscriber
class TestComponentPub : public MessageNode
{
    public:
        TestComponentPub(MessageNetwork* messageNetwork);
        
        void update();
};