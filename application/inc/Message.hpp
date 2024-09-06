#pragma once

#include "MessageTopicFlag.hpp"

class Message
{
    public:
        Message();

        void populateTopicList();
        void setMessageTopicFlag(MessageTopicFlag topicFlag);

        MessageTopicFlag getTopicFlag();
        std::vector< MessageTopic > getTopicList();

    private:
        MessageTopicFlag mTopicFlag;
        std::vector< MessageTopic > mTopicList;
};  