#pragma once

#include "utility/Logging/Logger.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include "core/Messaging/MessageTypes.hpp"

namespace Core {
class MessageNetwork;
class Message;

enum class NodeType { SUBSCRIBER = 0, PUBLISHER, SUB_AND_PUB };

/**
 * MessageNode is a base class for Publisher and Subscriber Nodes. The class registers the node onto
 * the Network object given in the constructor and facilitates communication to the Network.
 */
class MessageNode {
  public:
    MessageNode(MessageNetwork& messageNetwork, const std::string& messageNodeName,
                NodeType nodeType);

  protected:
    void addTopic(Messages::ID messageID);
    void addSubscriberTopic(Messages::ID messageID);
    void addPublisherTopic(Messages::ID messageID);

    void removeTopic(Messages::ID messageID);
    void removeSubscriberTopic(Messages::ID messageID);
    void removePublisherTopic(Messages::ID messageID);

    void unRegisterNode();
    void unRegisterSubscriberNode();
    void unRegisterpublisherNode();

    void publish(std::shared_ptr<Message> message);
    virtual void onNotify(Message* message);

  private:
    std::function<void(Message*)> getNotifyFunc();

  private:
    MessageNetwork& mMessageNetwork;

  protected:
    std::string mNodeName;
    std::function<void(Message*)> mCallback;
    NodeType mNodeType;

    std::shared_ptr<Utility::Logger> mNetworkLogger;
};

} // namespace Core
