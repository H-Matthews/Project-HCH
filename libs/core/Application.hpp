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
#include <optional>

namespace Core {

enum class ApplicationState {
    UNINITIALIZED = 0,
    INITIALIZED,
    RUNNING,
    SHUTTING_DOWN
};

std::string convertAppStateEnumToString(const ApplicationState& appState);
std::optional<ApplicationState> convertStringToAppStateEnum(std::string_view stringState);

class Application {
  public:
    static constexpr std::string_view SECTION_NAME = "Application";

    explicit Application(ConfigSpec configSpec);

    bool requestTransition(ApplicationState nextState);

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
    bool isValidTransition(ApplicationState from, ApplicationState to) const;

    inline bool isInitialized() const;
    inline bool isRunning() const;

  private:
    static const sf::Time TIME_PER_FRAME;

    std::unique_ptr<Configuration> mConfiguration;
    DirectoryLayout mDirectories;

    ApplicationState mState = ApplicationState::UNINITIALIZED;

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
