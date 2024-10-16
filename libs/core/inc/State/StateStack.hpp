#pragma once

#include "core/inc/State/State.hpp"

#include "utility/inc/StringOperations.hpp"

#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>

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
            void registerState(Core::States::ID stateID);

            void update(sf::Time fixedTimeStep);
            void draw();
            void handleEvent(const sf::Event& event);

            void pushState(Core::States::ID stateID);
            void popState();
            void clearStates();

            bool isEmpty() const;

        private:
            std::unique_ptr<Core::State> createState(Core::States::ID stateID);
            void applyPendingChanges();

            struct pendingStateRequests
            {
                explicit pendingStateRequests(Action action, Core::States::ID stateID = Core::States::NONE);

                Action action;
                Core::States::ID stateID;
            };

        private:
            std::vector<std::unique_ptr<Core::State>> mStack;
            std::vector<pendingStateRequests> mPendingList;
            Core::State::SharedObjects mSharedObjects;
            std::map<Core::States::ID, std::function<std::unique_ptr<Core::State>()>> mRegistry;
    };

}


template <typename T>
void Core::StateStack::registerState(Core::States::ID stateID)
{
    const std::string identifierString(Utility::statesEnumToString(stateID));

    // Stores a Lambda in mRegistry
    mRegistry[stateID] = [this, identifierString] ()
    {
        return std::unique_ptr<State>(new T(*this, identifierString, mSharedObjects));
    };
}