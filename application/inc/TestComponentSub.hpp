#include "MessageNode.hpp"

#include <iostream>

// This class is the publisher
class TestComponentSub : public MessageNode 
{
    public:
    TestComponentSub(MessageNetwork* messageNetwork);

    private:
        void onNotify(Message* message) override;
};