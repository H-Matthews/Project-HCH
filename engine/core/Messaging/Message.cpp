#include "core/Messaging/Message.hpp"

Core::Message::Info::Info() {
    sender = "";
    messageID = NO_MESSAGE;
}

Core::Message::Info::Info(const MessageID messageID, const std::string& stringMessageID)
    : messageID(messageID), stringMessageID(stringMessageID), sender("") {}

Core::Message::Message(const MessageID messageID, const std::string& stringMessageID)
    : mMessageInfo(messageID, stringMessageID) {}

Core::Message::~Message() {}

void Core::Message::setSender(const std::string& sender) {
    mMessageInfo.sender = sender;
}

std::string Core::Message::getSenderName() const {
    return mMessageInfo.sender;
}

Core::MessageID Core::Message::getMessageID() const {
    return mMessageInfo.messageID;
}

const std::string& Core::Message::getStringMessageID() const {
    return mMessageInfo.stringMessageID;
}