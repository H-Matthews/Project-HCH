#pragma once

#include <string_view>

namespace Core {
class Application;
}

/**
 * Keys for the game's resources. Core stores resources under string keys and knows nothing about
 * which ones exist; this is the game's list.
 */
namespace Fonts {
inline constexpr std::string_view ARIAL = "arial";
} // namespace Fonts

namespace Textures {
// The prototype draws with shapes, so these are not loaded yet. Reserved for sprite art.
inline constexpr std::string_view PLAYER = "player";
inline constexpr std::string_view ENEMY = "enemy";
inline constexpr std::string_view BACKGROUND = "background";
} // namespace Textures

namespace Application {

// Loads every resource the game needs. Throws std::runtime_error if a file is missing.
void loadGameResources(Core::Application& application);

} // namespace Application
