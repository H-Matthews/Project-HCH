#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
    public:
        Application();
        void run();

    private:
        void processInput();
        void update(sf::Time fixedTimeStep);
        void render();

    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow mWindow;

        // TODO: Implement a Resource Container for Textures / Fonts
};