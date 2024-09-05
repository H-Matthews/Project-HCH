#include "MessageNode.hpp"

#include <iostream>

// This class is the publisher
class TestComponentSub2 : public MessageNode 
{
    public:
    TestComponentSub2(MessageNetwork* messageNetwork);

    private:
        void onNotify(Message message) override;
};