#pragma once

#include "core/inc/State/StateTypes.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

namespace Core
{
    class StateStack;

    class State
    {
        public:

            struct SharedObjects
            {
                SharedObjects(sf::RenderWindow& window, std::string appLoggerName);

                sf::RenderWindow* window;
                std::string appLoggerName;
            };

        public:
            State(StateStack& stack, std::string mStateIdentifierString, SharedObjects sObjects);
            virtual ~State();

            virtual void draw() = 0;
            virtual bool update(sf::Time fixedTimeStep) = 0;
            virtual bool handleEvent(const sf::Event& event) = 0;

            const std::string getStateAsString();

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
