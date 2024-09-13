#include "MessageNode.hpp"
#include "PlayerActionMessage.hpp"
#include "EnemySpawnMessage.hpp"

/* This Class publishes the following Messages
1. PlayerActionMessages
2. EnemySpawnMessages
*/
class TestComponentPub : public MessageNode
{
    public:
        TestComponentPub(MessageNetwork* messageNetwork);
        void update();
};