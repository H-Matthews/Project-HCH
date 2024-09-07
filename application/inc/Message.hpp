#pragma once

#include "MessageTopic.hpp"

#include <vector>
#include <string>

class Message
{
    public:
        virtual ~Message();

        virtual void buildMessage(std::vector< MessageTopic > messageTopics, std::string sender) = 0;

        std::vector< MessageTopic > getTopicList();
        std::string getSenderName();

    protected:
        std::vector< MessageTopic > mTopicList;
        std::string mSender;
};  