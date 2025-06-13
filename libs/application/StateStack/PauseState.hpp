#include "core/StateStack/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace Application
{
    class PauseState : public Core::State
    {
      public:
        PauseState( Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects );

        virtual void draw();

        virtual bool update( sf::Time fixedTimeStep );
        bool handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent ) override;
    };
}
