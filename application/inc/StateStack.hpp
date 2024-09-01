#pragma once

#include "State.hpp"

#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>

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
        explicit StateStack(State::SharedObjects sObjects);

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

    private:
        std::unique_ptr<State> createState(States::ID stateID);
        void applyPendingChanges();

        struct pendingStateRequests
        {
            explicit pendingStateRequests(Action action, States::ID stateID = States::None);

            Action action;
            States::ID stateID;
        };

    private:
        std::vector<std::unique_ptr<State>> mStack;
        std::vector<pendingStateRequests> mPendingList;
        std::map<States::ID, std::function<std::unique_ptr<State>()>> mRegistry;

        State::SharedObjects mSharedObjects;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
    // Stores a Lambda in mRegistry
    mRegistry[stateID] = [this] ()
    {
        return std::unique_ptr<State>(new T(*this, mSharedObjects));
    };
}