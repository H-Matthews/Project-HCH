#include "MessageNode.hpp"

#include <iostream>
#include <memory>

// This class will be the Subscriber
class TestComponentPub : public MessageNode
{
    public:
        TestComponentPub(MessageNetwork* messageNetwork);
        
        void update();
};