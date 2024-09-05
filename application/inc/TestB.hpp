#include "MessageNode.hpp"

#include <iostream>

// This class is the publisher
class TestB : public MessageNode 
{
    public:
    TestB(MessageNetwork* messageNetwork);
    void update();

    private:
        void onNotify(Message message);
};