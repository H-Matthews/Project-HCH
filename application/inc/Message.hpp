#pragma once

#include "MessageTopic.hpp"

#include <vector>

class Message
{
    public:
        Message();
        Message(Message* message);

        void buildMessage(std::vector< MessageTopic >);

        std::vector< MessageTopic > getTopicList();

    private:
        void buildMessageHeader();

    private:
        std::vector< MessageTopic > mTopicList;
};  