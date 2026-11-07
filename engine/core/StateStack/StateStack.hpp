#pragma once

#include "core/Event.hpp"
#include "core/Context.hpp"
#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include "utility/Logging/Logger.hpp"

#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <map>
#include <memory>

#include <iostream>

namespace Core {
class Application;
class State;

/**
 * PROPAGATION RULE
 *
 * The stack is walked top-down (most recently pushed state first). A state is reached unless a
 * state ABOVE it blocks that channel. Each State declares what it blocks:
 *
 *   blocksInput()    -> states below receive no events, and their input() reports every key up
 *   blocksUpdates()  -> states below are not updated (they are still drawn, bottom-up)
 *
 * The state that blocks is itself still reached. Example, with a Pause State on top of Game:
 *
 *   Pause (blocks input + updates)  -> gets events, input and update
 *   Game                            -> gets nothing; frozen and deaf until Pause is popped
 *
 * The whole rule lives in StateStack::handleEvent(), StateStack::update() and
 * StateStack::refreshInputGating().
 */
class StateStack {
  public:
    static constexpr std::string_view SECTION_NAME = "StateStack";

    enum class Action { PUSH = 0, POP, CLEAR };

  public:
    explicit StateStack(const ConfigSection* config, Context ctx);
    ~StateStack();

    void update(sf::Time fixedTimeStep);
    void draw() const;
    void handleEvent(const Event& event);

    template <typename TState>
    requires(std::is_base_of_v<State, TState> &&
        std::constructible_from<TState, StateStack&, Context>)
    void pushState();

    void popState();
    void clearStates();

    bool isEmpty() const;
    bool isPendingListEmpty() const;

    void initializeLogger();

  private:
    void applyPendingChanges();

    // Enables or disables each state's InputView according to the propagation rule
    void refreshInputGating();

    struct PendingStateRequest {
        explicit PendingStateRequest(Action action);

        Action action;
        std::function<std::unique_ptr<State>()> stateConstructor;
    };

    std::unique_ptr<State> createState(const PendingStateRequest& changeRequest);

  private:
    const ConfigSection* mConfig;

    std::vector<std::unique_ptr<State>> mStack;
    std::vector<PendingStateRequest> mPendingRequests;
    Context mContext;

    std::shared_ptr<Utility::Logger> mLogger;
};

template <typename TState>
requires(std::is_base_of_v<State, TState> && std::constructible_from<TState, StateStack&, Context>)
void StateStack::pushState() {
    PendingStateRequest request(Action::PUSH);
    request.stateConstructor = [this, ctx = mContext]() {
        return std::unique_ptr<State>(new TState(*this, ctx));
    };
    mPendingRequests.push_back(request);
}

} // namespace Core
