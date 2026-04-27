#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include "utility/Logging/Logger.hpp"

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

    class StateStack
    {
      public:
        static constexpr std::string_view SECTION_NAME = "StateStack";

        enum class Action
        {
            PUSH = 0,
            POP,
            CLEAR
        };

      public:
        explicit StateStack( Application& application, const ConfigSection* config = nullptr );
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

        void initializeLogger();

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
        const ConfigSection* mConfig;

        Application& applicationRef;

        std::shared_ptr< Utility::Logger > mLogger;
    };

    template < typename TState >
        requires( std::is_base_of_v< Core::State, TState > )
    void StateStack::pushState()
    {
        PendingStateRequest request( Action::PUSH );
        request.stateConstructor = []() { return std::unique_ptr< Core::State >( new TState() ); };
        mPendingRequests.push_back( request );
    }
}
