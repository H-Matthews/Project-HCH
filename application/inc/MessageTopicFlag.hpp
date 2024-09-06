#pragma once

#include "stdint.h"
#include <vector>

enum class MessageTopic
{
    PLAYER = 1 << 0, // 1
    ENEMY  = 1 << 1, // 2
    DROPS  = 1 << 2, // 4
};

class MessageTopicFlag 
{
    public:
        MessageTopicFlag();

        void setFlag(MessageTopic flag);
        void unsetFlag(MessageTopic flag);
        void resetFlag();
        bool hasFlag(MessageTopic flag);
        uint8_t getFlagValue();

        std::vector< MessageTopic > determineTopics();
    
    private:
        uint8_t mFlagValue;
};