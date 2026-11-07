#pragma once

#include "core/Event.hpp"

#include <SFML/Window/Event.hpp>

#include <optional>

namespace Core {
std::optional<Event> translate(const sf::Event& sfEvent);

} // NAMESPACE CORE