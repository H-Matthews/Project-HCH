#include "MessageNode.hpp"

#include <iostream>

// This class will be the Subscriber
class TestA : public MessageNode
{
    public:
    TestA(MessageNetwork* messageNetwork);

    private:
        void onNotify(Message message);
};