#include "MessageNode.hpp"
#include "EnemySpawnMessage.hpp"

#include <iostream>

/* This class only handles the following Messages
1. EnemySpawnMessage
*/
class TestComponentSub2 : public MessageNode 
{
    public:
    TestComponentSub2(MessageNetwork* messageNetwork);

    private:
        void onNotify(Message* message) override;
};