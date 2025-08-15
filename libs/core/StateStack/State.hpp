#pragma once

#include "core/StateStack/StateTypes.hpp"
#include "core/Messaging/MessageNetwork.hpp"

#include "core/GameAssetContainer/ResourceEnums.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>

#include <string>

namespace Core
{
    class StateStack;

    class State
    {
      public:
        struct SharedObjects
        {
            SharedObjects( sf::RenderWindow& window, Core::MessageNetwork& network, TextureHolder& textures );
            SharedObjects();

            sf::RenderWindow* window;
            Core::MessageNetwork* network;
            TextureHolder* textures;
        };

      public:
        State( StateStack& stack, std::string mStateIdentifierString, SharedObjects sObjects );
        virtual ~State();

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

        const std::string getStateAsString();

      protected:
        void requestStackPush( States::ID stateID );
        void requestStackPop();
        void requestStateClear();

        SharedObjects getSharedObjects() const;

      private:
        StateStack* mStack;
        std::string mStateIdentifierString;
        SharedObjects mSharedObjects;
    };

}
