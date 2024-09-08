#pragma once

#include "MessageNetwork.hpp"

#include <string>

class MessageNetwork;

class MessageNode 
{
    public:
        MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName);
        MessageNode(MessageNetwork* messageNetwork);

        std::unique_ptr<Message> createMessage(Messages::ID messageID);

        void setPublishMessage(Messages::ID publishMessageID);
        void setSubscribeMessage(Messages::ID subscribeMessageID);
        void setNodeName(std::string nodeName);
        const std::set< Messages::ID >& getSubscribeToMessageList() const;
        const Messages::ID& getPublishMessageID() const;
        const std::string& getNodeName() const;

    protected:
        void registerSubscriberMessages();
        void send(Message* message);
        virtual void onNotify(Message* message);

    private:
        std::function<void (Message*)> getNotifyFunc();

    private:
        MessageNetwork* mMessageNetwork;
        MessageNodeInfo mMessageNodeInfo;
        std::set< Messages::ID > mSubscribeToMessages;
        Messages::ID mPublishMessageID;
};