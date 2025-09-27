#pragma once

#include "core/StateStack/State.hpp"
#include "core/Configuration/Configurables/Configurable.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>

namespace Core
{
    class Application;

    class StateStack : public Configurable
    {
      public:
        static const std::string TYPE_NAME;

        enum Action
        {
            Push,
            Pop,
            Clear
        };

      public:
        StateStack( Application& application );

        void update( sf::Time fixedTimeStep );
        void draw();

        void handleKeyPressed( const sf::Event::KeyPressed& keyPressed );
        void handleMouseMoved( const sf::Event::MouseMoved& mouseMoved );
        void handleRealTimeInput();

        void registerState(
            const std::string& stateIdentifier, std::function< std::unique_ptr< Core::State >() > registerFunc );

        void pushState( const std::string& stateIdentifier );
        void popState();
        void clearStates();

        bool isEmpty() const;
        bool isPendingListEmpty() const;

        void initializeLogger();

      private:
        std::unique_ptr< Core::State > createState( std::string stateIdentifier );
        void applyPendingChanges();

        struct PendingStateRequest
        {
            explicit PendingStateRequest( Action action, const std::string& stateIdentifier = "" );

            Action action;
            std::string stateIdentifier;
        };

      private:
        std::vector< std::unique_ptr< Core::State > > mStack;
        std::vector< PendingStateRequest > mPendingRequests;
        std::map< std::string, std::function< std::unique_ptr< Core::State >() > > mRegistry;

        Application& applicationRef;
    };

}