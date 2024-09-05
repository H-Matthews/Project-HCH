#pragma once

#include "MessageTopicFlag.hpp"

class Message
{
    public:
        Message(MessageTopicFlag topic);

        void determineTopics();
        std::vector< MessageTopic > getTopicList();

    private:
        MessageTopicFlag mTopicFlag;
        std::vector< MessageTopic > mTopicList;
};  