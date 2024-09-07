#pragma once

#include "MessageTopic.hpp"

#include <vector>

class Message
{
    public:
        Message();

        void buildMessage(std::vector< MessageTopic >);

        std::vector< MessageTopic > getTopicList();

    private:
        std::vector< MessageTopic > mTopicList;
};  