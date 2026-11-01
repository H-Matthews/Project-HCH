#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace Application::UI {

// Builds a text object; when `centered` the position is the middle of the text
inline sf::Text makeText(const sf::Font& font, const std::string& string, unsigned int size,
                         sf::Vector2f position, sf::Color color = sf::Color::White,
                         bool centered = false) {
    sf::Text text(font, string, size);
    text.setFillColor(color);
    if (centered) {
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(
            {bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    }
    text.setPosition(position);
    return text;
}

// Full-window translucent rectangle (call with the default view active)
inline sf::RectangleShape makeOverlay(sf::Vector2f windowSize, sf::Color color) {
    sf::RectangleShape overlay(windowSize);
    overlay.setFillColor(color);
    return overlay;
}

} // namespace Application::UI
