#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork)
{
    MessageNode::mMessageNodeInfo.nodeName = "TestComponentPublish";
}

void TestComponentPub::update()
{
    auto msg = createMessage(Message::ID::PlayerActionMessage);

    // Must cast to set properties for derived class
    PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>( msg.get() );
    if(pam)
    {
        pam->action = Action::MOVE_DOWN;
    }

    send(msg.get());
}