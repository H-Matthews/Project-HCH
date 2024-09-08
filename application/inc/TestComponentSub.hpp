#include "MessageNode.hpp"

#include <iostream>

/* This class only handles the following Messages
1. PlayerActionMessage
*/
class TestComponentSub : public MessageNode 
{
    public:
    TestComponentSub(MessageNetwork* messageNetwork);

    private:
        void onNotify(Message* message) override;
};