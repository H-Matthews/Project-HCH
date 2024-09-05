#include "Message.hpp"

Message::Message(MessageFlag messageRecipients, MessageType messageType) :
mMessageRecipients(messageRecipients),
mMessageType(messageType)
{
}