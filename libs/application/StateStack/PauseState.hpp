#include "core/StateStack/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <string>

namespace Application
{
    class PauseState : public Core::State
    {
      public:
        static std::string identifier;

        PauseState( std::string stateIdentifier );

        virtual void draw();

        virtual bool update( sf::Time fixedTimeStep );
        bool handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent ) override;
    };
}
