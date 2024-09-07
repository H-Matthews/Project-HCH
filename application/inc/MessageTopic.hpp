#pragma once

#include "stdint.h"

constexpr uint8_t MESSAGE_TOPIC_SIZE = 3;

enum class MessageTopic
{
    PLAYER = 1 << 0, // 1
    ENEMY  = 1 << 1, // 2
    DROPS  = 1 << 2 // 4
};