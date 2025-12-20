#pragma once

#include "core/Configuration/Configurables/Configurable.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>

#include <iostream>

namespace Core
{
    class Application;
    class State;
    class MessageNetwork;

    class StateStack : public Configurable
    {
      public:
        static const std::string TYPE_NAME;

        enum Action
        {
            PUSH = 0,
            POP,
            CLEAR
        };

      public:
        StateStack( Application& application );
        ~StateStack();

        void update( sf::Time fixedTimeStep );
        void draw();

        void handleKeyPressed( const sf::Event::KeyPressed& keyPressed );
        void handleMouseMoved( const sf::Event::MouseMoved& mouseMoved );
        void handleRealTimeInput();

        template < typename TState >
            requires( std::is_base_of_v< Core::State, TState > )
        void pushState();

        void popState();
        void clearStates();

        bool isEmpty() const;
        bool isPendingListEmpty() const;

        MessageNetwork* getMessageNetworkRef();

      private:
        void applyPendingChanges();

        struct PendingStateRequest
        {
            explicit PendingStateRequest( Action action );

            Action action;
            std::function< std::unique_ptr< Core::State >() > stateConstructor;
        };

        std::unique_ptr< Core::State > createState( const Core::StateStack::PendingStateRequest& changeRequest );

      private:
        std::vector< std::unique_ptr< Core::State > > mStack;
        std::vector< PendingStateRequest > mPendingRequests;

        Application& applicationRef;
    };

    template < typename TState >
        requires( std::is_base_of_v< Core::State, TState > )
    void StateStack::pushState()
    {

        PendingStateRequest request( Action::PUSH );
        request.stateConstructor = []() { return std::unique_ptr< Core::State >( new TState() ); };

        mPendingRequests.push_back( request );

        return;
    }

}