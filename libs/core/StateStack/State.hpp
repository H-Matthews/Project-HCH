#pragma once

#include "core/Messaging/MessageNetwork.hpp"
#include "core/StateStack/StateStack.hpp"

#include "core/GameAssetContainer/ResourceEnums.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>

#include <string>

namespace Core
{

    class State
    {
      public:
        State( std::string stateIdentifier );
        virtual ~State();

        // To be called after setting the StateStack reference
        virtual bool initializeState()
        {
            return true;
        }

        virtual void draw() = 0;
        virtual bool update( sf::Time fixedTimeStep ) = 0;
        virtual bool handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent ) = 0;
        virtual bool handleMouseMoved( const sf::Event::MouseMoved& )
        {
            return false;
        }
        virtual bool handleRealTimeInput()
        {
            return false;
        }

        const std::string getStateName();
        void setStackRef( StateStack* stack );

      protected:
        template < typename TState >
            requires( std::is_base_of_v< Core::State, TState > )
        void requestStackPush();

        void requestStackPop();
        void requestStateClear();

      protected:
        StateStack* mStackRef;

      private:
        std::string mStateIdentifierString;
    };

    template < typename TState >
        requires( std::is_base_of_v< Core::State, TState > )
    void State::requestStackPush()
    {
        mStackRef->pushState< TState >();

        return;
    }

}
