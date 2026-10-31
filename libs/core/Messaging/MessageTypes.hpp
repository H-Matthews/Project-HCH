#pragma once

#include <cstdint>

namespace Core {
/**
 * Identifies a kind of message (a "topic") on the MessageNetwork.
 *
 * Core does not know which messages exist: the game layer defines its own IDs (an enum or
 * constants) and passes them in. 0 is reserved for "no message".
 */
using MessageID = std::uint32_t;

inline constexpr MessageID NO_MESSAGE = 0;
} // namespace Core
