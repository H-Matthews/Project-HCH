#include <gtest/gtest.h>

#include "core/Context.hpp"
#include "core/Event.hpp"
#include "core/InputState.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/StateStack/State.hpp"
#include "core/StateStack/StateStack.hpp"
#include "core/Window/Window.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <vector>

namespace {

// The stack only stores a Window reference in Context; it never calls it
class StubWindow : public Core::Window {
  public:
    bool initialize(const Core::ConfigSection&) override { return true; }
    void close() override {}
    bool isOpen() const override { return true; }
    void shutdown() override {}
    std::optional<Core::Event> pollEvent() override { return std::nullopt; }
    void swapBuffers() override {}
    uint32_t getWidth() const override { return 0; }
    uint32_t getHeight() const override { return 0; }
    void* getNativeWindowHandle() const override { return nullptr; }
};

// Every call made by the stack is recorded here, in call order
std::vector<std::string> gLog;

// A state with a configurable name and blocking policy. Its update() records whether the W key
// looks down through its InputView.
template <char Name, bool BlockInput, bool BlockUpdates> class TestState : public Core::State {
  public:
    TestState(Core::StateStack& stack, Core::Context ctx) : State(stack, ctx) {}

    void handleEvent(const Core::Event&) override {
        gLog.push_back(std::string(1, Name) + ":event");
    }

    void update(sf::Time) override {
        gLog.push_back(std::string(1, Name) +
                       ":update:" + (input().isDown(Core::Key::W) ? "down" : "up"));
    }

    void draw() override { gLog.push_back(std::string(1, Name) + ":draw"); }

    std::string_view getStateType() override { return "Test"; }

    bool blocksInput() const override { return BlockInput; }
    bool blocksUpdates() const override { return BlockUpdates; }
};

using Plain = TestState<'B', false, false>;        // bottom state
using Passive = TestState<'T', false, false>;      // overlay that blocks nothing
using InputBlocker = TestState<'T', true, false>;  // overlay that blocks input only
using UpdateBlocker = TestState<'T', false, true>; // overlay that blocks updates only
using FullBlocker = TestState<'T', true, true>;    // pause-style overlay

// Asks the stack for another state from inside initializeState()
class Pusher : public Core::State {
  public:
    Pusher(Core::StateStack& stack, Core::Context ctx) : State(stack, ctx) {}

    bool initializeState() override {
        requestStackPush<Plain>();
        return true;
    }

    void update(sf::Time) override {}
    void draw() override { gLog.push_back("P:draw"); }
    std::string_view getStateType() override { return "Pusher"; }
};

using Log = std::vector<std::string>;

class StateStackTest : public ::testing::Test {
  protected:
    StateStackTest() : stack(nullptr, Core::Context{window, nullptr, input, fonts, textures}) {
        gLog.clear();
        input.update(Core::KeyPressed{Core::Key::W, {}});
    }

    // pushState is deferred until the end of the next update()
    template <typename T> void push() {
        stack.pushState<T>();
        stack.update(sf::Time::Zero);
        gLog.clear();
    }

    StubWindow window;
    Core::InputState input;
    FontHolder fonts;
    TextureHolder textures;
    Core::StateStack stack;
};

} // namespace

TEST_F(StateStackTest, nonBlockingOverlayReachesEveryState) {
    push<Plain>();
    push<Passive>();

    stack.handleEvent(Core::FocusGained{});
    stack.update(sf::Time::Zero);

    EXPECT_EQ(gLog, (Log{"T:event", "B:event", "T:update:down", "B:update:down"}));
}

TEST_F(StateStackTest, inputBlockerSilencesStatesBelowButNotUpdates) {
    push<Plain>();
    push<InputBlocker>();

    stack.handleEvent(Core::FocusGained{});
    stack.update(sf::Time::Zero);

    // Events stop at the blocker; the lower state still updates but sees every key up
    EXPECT_EQ(gLog, (Log{"T:event", "T:update:down", "B:update:up"}));
}

TEST_F(StateStackTest, updateBlockerFreezesStatesBelowButTheyStillDraw) {
    push<Plain>();
    push<UpdateBlocker>();

    stack.handleEvent(Core::FocusGained{});
    stack.update(sf::Time::Zero);
    stack.draw();

    // Events still reach the lower state; it is not updated, and draw runs bottom-up
    EXPECT_EQ(gLog, (Log{"T:event", "B:event", "T:update:down", "B:draw", "T:draw"}));
}

TEST_F(StateStackTest, fullBlockerFreezesAndSilencesStatesBelow) {
    push<Plain>();
    push<FullBlocker>();

    stack.handleEvent(Core::FocusGained{});
    stack.update(sf::Time::Zero);

    EXPECT_EQ(gLog, (Log{"T:event", "T:update:down"}));
}

TEST_F(StateStackTest, blockDoesNotSkipStatesTwoLevelsDown) {
    push<Plain>();
    push<Passive>();
    push<InputBlocker>();

    stack.update(sf::Time::Zero);

    // Passive does not lift the block: both states under the blocker see keys up
    EXPECT_EQ(gLog, (Log{"T:update:down", "T:update:up", "B:update:up"}));
}

TEST_F(StateStackTest, stateBecomesReachableAgainAfterBlockerIsPopped) {
    push<Plain>();
    push<FullBlocker>();

    stack.update(sf::Time::Zero);
    stack.popState();
    stack.update(sf::Time::Zero); // applies the pop at the end of the update
    gLog.clear();

    stack.handleEvent(Core::FocusGained{});
    stack.update(sf::Time::Zero);

    EXPECT_EQ(gLog, (Log{"B:event", "B:update:down"}));
}

TEST_F(StateStackTest, popOnEmptyStackIsIgnored) {
    stack.popState();
    stack.update(sf::Time::Zero);

    EXPECT_TRUE(stack.isEmpty());
}

TEST_F(StateStackTest, extraPopsAreIgnored) {
    push<Plain>();

    stack.popState();
    stack.popState();
    stack.update(sf::Time::Zero);
    stack.update(sf::Time::Zero);

    EXPECT_TRUE(stack.isEmpty());
}

TEST_F(StateStackTest, pushFromInitializeStateIsDeferredOneUpdate) {
    stack.pushState<Pusher>();
    stack.update(sf::Time::Zero);

    // First update applied only the original push; the one made in initializeState() waits
    EXPECT_FALSE(stack.isPendingListEmpty());
    gLog.clear();
    stack.draw();
    EXPECT_EQ(gLog, (Log{"P:draw"}));

    stack.update(sf::Time::Zero);

    EXPECT_TRUE(stack.isPendingListEmpty());
    gLog.clear();
    stack.draw();
    EXPECT_EQ(gLog, (Log{"P:draw", "B:draw"}));
}
