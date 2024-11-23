#pragma once

#include "core/inc/State/State.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <sstream>

namespace Core
{
    
    class StateStack 
    {
        public:
            enum Action
            {
                Push, 
                Pop,
                Clear
            };

        public:
            explicit StateStack(Core::State::SharedObjects sObjects);

            // Needs to be a template so that we can treat registerState as a factory
            template <typename T>
            void registerState(States::ID stateID);

            void update(sf::Time fixedTimeStep);
            void draw();
            void handleEvent(const sf::Event& event);

            void pushState(States::ID stateID);
            void popState();
            void clearStates();

            bool isEmpty() const;

            void initializeLogger();

        private:
            std::unique_ptr<Core::State> createState(States::ID stateID);
            void applyPendingChanges();

            struct pendingStateRequests
            {
                explicit pendingStateRequests(Action action, States::ID stateID = States::NONE);

                Action action;
                States::ID stateID;
            };

        private:
            std::shared_ptr< Utility::Logger > mLogger;

            std::vector<std::unique_ptr<Core::State>> mStack;
            std::vector<pendingStateRequests> mPendingList;
            Core::State::SharedObjects mSharedObjects;
            std::map<States::ID, std::function<std::unique_ptr<Core::State>()>> mRegistry;

    };

}


template <typename T>
void Core::StateStack::registerState(States::ID stateID)
{
    const std::string identifierString(States::statesEnumToString(stateID));

    // Stores a Lambda in mRegistry
    mRegistry[stateID] = [this, identifierString] ()
    {
        return std::unique_ptr<State>(new T(*this, identifierString, mSharedObjects));
    };

    std::stringstream logStream;
    logStream << "Registered State: " << identifierString << " on the StateStack";

    mLogger->logInfo(logStream.str());
}