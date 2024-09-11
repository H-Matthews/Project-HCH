#include "MessageNode.hpp"
#include "PlayerActionMessage.hpp"
#include "EnemySpawnMessage.hpp"

// This class will be the Subscriber
class TestComponentPub : public MessageNode
{
    public:
        TestComponentPub(MessageNetwork* messageNetwork);
        
        void update();
};