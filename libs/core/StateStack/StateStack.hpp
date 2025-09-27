#pragma once

#include "core/StateStack/State.hpp"
#include "core/Configuration/Configurables/Configurable.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>

namespace Core
{

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
        StateStack();

        void update( sf::Time fixedTimeStep );
        void draw();

        void handleKeyPressed( const sf::Event::KeyPressed& keyPressed );
        void handleMouseMoved( const sf::Event::MouseMoved& mouseMoved );
        void handleRealTimeInput();

        void registerState( const std::string& stateIdentifier, std::function< Core::State*() > registerFunc );

        void pushState( const std::string& stateIdentifier );
        void popState();
        void clearStates();

        bool isEmpty() const;
        bool isPendingListEmpty() const;

        void initializeLogger();

      private:
        Core::State* createState( std::string stateIdentifier );
        void applyPendingChanges();

        struct PendingStateRequest
        {
            explicit PendingStateRequest( Action action, const std::string& stateIdentifier = "" );

            Action action;
            std::string stateIdentifier;
        };

      private:
        std::vector< Core::State* > mStack;
        std::vector< PendingStateRequest > mPendingStateList;
        std::map< std::string, std::function< Core::State*() > > mRegistry;
    };

}