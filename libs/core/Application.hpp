#pragma once

#include "application/Settings/KeyBindings.hpp"

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/DirectoryLayout.hpp"
#include "core/StateStack/StateStack.hpp"
#include "core/Messaging/MessageNetwork.hpp"
#include "core/GameAssetContainer/GameAssetContainer.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/GameAssetContainer/ResourceEnums.hpp"

#include "utility/Logging/Logger.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <functional>

namespace Core {
class Application {
  public:
    enum class State { NONE = 0, INITIALIZED, RUNNING, SHUTTING_DOWN };

    std::string convertAppStateEnumToString(const State& state) const;
    State convertStringToAppStateEnum(std::string_view stringState) const;

  public:
    static constexpr std::string_view SECTION_NAME = "Application";

    explicit Application(ConfigSpec configSpec);

    MessageNetwork* getNetwork();

    void registerState(const std::string& stateIdentifier,
                       std::function<std::unique_ptr<Core::State>()> registerFunc);

    template <typename TState> requires(std::is_base_of_v<Core::State, TState>) void pushState();

    void initialize();
    void run();

  private:
    void processInput();
    void update(sf::Time fixedTimeStep);
    void render();

    void loadResources();
    void buildSubsystemLoggers();

    bool transitionState(State statusToTransfer);

    inline bool isInitialized() const;
    inline bool isRunning() const;

  private:
    static const sf::Time TIME_PER_FRAME;

    std::unique_ptr<Configuration> mConfiguration;
    DirectoryLayout mDirectories;

    State mState;

    // Should this go here? or in the GameState?
    TextureHolder mTextures;

    // CORE APPLICATION CLASSES
    MessageNetwork mNetwork;
    StateStack mStateStack;

    // TODO: These need to go into a CORE Class
    // Application::KeyBindings mPlayerKeyBindings;
    sf::RenderWindow mWindow;

    std::shared_ptr<Utility::Logger> mLogger;
};

inline MessageNetwork* Application::getNetwork() {
    return &mNetwork;
}

bool Application::isInitialized() const {
    return mState == State::INITIALIZED;
}

bool Application::isRunning() const {
    return mState == State::RUNNING;
}

template <typename TState>
requires(std::is_base_of_v<Core::State, TState>) void Application::pushState() {
    if ((int)mState <= 0)
        return;

    mStateStack.pushState<TState>();
}
} // namespace Core
