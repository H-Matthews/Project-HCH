#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

namespace Core
{
    class StateStack;

    namespace States
    {
        enum ID
        {
            NONE = 0,
            Menu,
            Game, 
            Pause
        };
    }

    class State
    {
        public:

            struct SharedObjects
            {
                SharedObjects(sf::RenderWindow& window);

                sf::RenderWindow* window;
            };

        public:
            State(StateStack& stack, std::string mStateIdentifierString, SharedObjects sObjects);
            virtual ~State();

            virtual void draw() = 0;
            virtual bool update(sf::Time fixedTimeStep) = 0;
            virtual bool handleEvent(const sf::Event& event) = 0;

        protected:
            void requestStackPush(States::ID stateID);
            void requestStackPop();
            void requestStateClear();

            SharedObjects getSharedObjects() const;

        private:
            StateStack* mStack;
            std::string mStateIdentifierString;
            SharedObjects mSharedObjects;
    };

}
