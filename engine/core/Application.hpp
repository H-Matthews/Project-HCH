#pragma once

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/DirectoryLayout.hpp"
#include "core/StateStack/StateStack.hpp"
#include "core/Messaging/MessageNetwork.hpp"
#include "core/Window/Window.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/GameAssetContainer/ResourceTypes.hpp"

#include "utility/Logging/Logger.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <functional>
#include <optional>

namespace Core {

enum class ApplicationState { UNINITIALIZED = 0, INITIALIZED, RUNNING, SHUTTING_DOWN };

std::string convertAppStateEnumToString(const ApplicationState& appState);
std::optional<ApplicationState> convertStringToAppStateEnum(std::string_view stringState);

struct AppSpec {
    ConfigSpec configSpec;
    WindowSpec windowSpec;

    explicit AppSpec(ConfigSpec cSpec, WindowSpec wSpec) :
        configSpec(std::move(cSpec)),
        windowSpec(std::move(wSpec)) {}

    AppSpec(const AppSpec&) = delete;
    AppSpec& operator=(const AppSpec&) = delete;
    AppSpec(AppSpec&&) = default;
    AppSpec& operator=(AppSpec&&) = default;
};

class SfmlWindow;

class Application {
  public:
    static constexpr std::string_view SECTION_NAME = "Application";

    explicit Application(AppSpec appSpec);

    bool requestTransition(ApplicationState nextState);

    // Top-level table from the loaded TOML files, or nullptr if it does not exist
    const ConfigSection* getConfigSection(std::string_view name) const {
        return mConfiguration->getSection(name);
    }

    // Subsystem Getters
    MessageNetwork& getNetwork() { return mNetwork; }
    Window& getWindow() { return *mWindow; }
    TextureHolder& getTextures() { return mTextures; }
    FontHolder& getFonts() { return mFonts; }
    const DirectoryLayout& getDirectories() const { return mDirectories; }

    void registerState(const std::string& stateIdentifier,
                       std::function<std::unique_ptr<Core::State>()> registerFunc);

    template <typename TState> requires(std::is_base_of_v<Core::State, TState>) void pushState();

    void initialize();
    void run();

  private:
    void processRealtimeInput();
    void update(sf::Time fixedTimeStep);
    void render();

    void handleEvent(const sf::Event& event);

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

    TextureHolder mTextures;
    FontHolder mFonts;

    // CORE APPLICATION CLASSES
    MessageNetwork mNetwork;

    std::unique_ptr<Window> mWindow;

    // Unforuntaely necessary for now
    SfmlWindow* sfmlWindow = nullptr;

    StateStack mStateStack;

    std::shared_ptr<Utility::Logger> mLogger;
};

bool Application::isInitialized() const {
    return mState == ApplicationState::INITIALIZED;
}

bool Application::isRunning() const {
    return mState == ApplicationState::RUNNING;
}

template <typename TState>
requires(std::is_base_of_v<Core::State, TState>) void Application::pushState() {
    if ((int)mState <= 0) {
        return;
    }

    mStateStack.pushState<TState>();
}
} // namespace Core
