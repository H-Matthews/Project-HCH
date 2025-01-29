#include "application/inc/App.hpp"

#include "core/inc/Configuration.hpp"

int main()
{

    auto config = std::make_shared<Core::Configuration>();

    Application::App game(config);
    if(game.initialize())
        game.run();

    return 0;
}