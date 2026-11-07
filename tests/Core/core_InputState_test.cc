#include <gtest/gtest.h>

#include "core/Event.hpp"
#include "core/InputState.hpp"
#include "core/StateStack/InputView.hpp"

using Core::InputState;
using Core::InputView;
using Core::Key;
using Core::MouseButton;

namespace {
Core::KeyPressed press(Key key, bool repeat = false) { return Core::KeyPressed{key, {}, repeat}; }
Core::KeyReleased release(Key key) { return Core::KeyReleased{key, {}}; }
} // namespace

TEST(Core_InputState_Test, pressSetsHeldAndPressedEdge) {
    InputState input;

    input.update(press(Key::Space));

    EXPECT_TRUE(input.isDown(Key::Space));
    EXPECT_TRUE(input.wasPressed(Key::Space));
    EXPECT_FALSE(input.wasReleased(Key::Space));
}

TEST(Core_InputState_Test, clearEdgesKeepsHeldButDropsEdges) {
    InputState input;
    input.update(press(Key::Space));

    input.clearEdges();

    EXPECT_TRUE(input.isDown(Key::Space));
    EXPECT_FALSE(input.wasPressed(Key::Space));
}

TEST(Core_InputState_Test, releaseClearsHeldAndSetsReleasedEdge) {
    InputState input;
    input.update(press(Key::Space));
    input.clearEdges();

    input.update(release(Key::Space));

    EXPECT_FALSE(input.isDown(Key::Space));
    EXPECT_TRUE(input.wasReleased(Key::Space));
    EXPECT_FALSE(input.wasPressed(Key::Space));
}

TEST(Core_InputState_Test, tapWithinOneUpdateIsNotLost) {
    InputState input;

    input.update(press(Key::Space));
    input.update(release(Key::Space));

    EXPECT_FALSE(input.isDown(Key::Space));
    EXPECT_TRUE(input.wasPressed(Key::Space));
    EXPECT_TRUE(input.wasReleased(Key::Space));
}

TEST(Core_InputState_Test, repeatedKeyPressDoesNotSetPressedEdge) {
    InputState input;
    input.update(press(Key::Space));
    input.clearEdges();

    input.update(press(Key::Space, /*repeat=*/true));

    EXPECT_TRUE(input.isDown(Key::Space));
    EXPECT_FALSE(input.wasPressed(Key::Space));
}

TEST(Core_InputState_Test, mouseButtonsHaveEdgesToo) {
    InputState input;

    input.update(Core::MousePressed{MouseButton::Left, 10, 20});
    EXPECT_TRUE(input.isDown(MouseButton::Left));
    EXPECT_TRUE(input.wasPressed(MouseButton::Left));

    input.clearEdges();
    input.update(Core::MouseReleased{MouseButton::Left, 10, 20});
    EXPECT_FALSE(input.isDown(MouseButton::Left));
    EXPECT_TRUE(input.wasReleased(MouseButton::Left));
}

TEST(Core_InputState_Test, focusLostClearsHeldStateAndEdges) {
    InputState input;
    input.update(press(Key::W));
    input.update(Core::MousePressed{MouseButton::Right, 0, 0});

    input.update(Core::FocusLost{});

    EXPECT_FALSE(input.isDown(Key::W));
    EXPECT_FALSE(input.wasPressed(Key::W));
    EXPECT_FALSE(input.isDown(MouseButton::Right));
    EXPECT_FALSE(input.wasPressed(MouseButton::Right));
}

TEST(Core_InputView_Test, enabledViewForwardsEverything) {
    InputState input;
    input.update(press(Key::W));
    InputView view(input);

    EXPECT_TRUE(view.isDown(Key::W));
    EXPECT_TRUE(view.wasPressed(Key::W));
}

TEST(Core_InputView_Test, disabledViewReportsEverythingUp) {
    InputState input;
    input.update(press(Key::W));
    input.update(release(Key::A));
    input.update(Core::MousePressed{MouseButton::Left, 0, 0});
    InputView view(input);

    view.setEnabled(false);

    EXPECT_FALSE(view.isDown(Key::W));
    EXPECT_FALSE(view.wasPressed(Key::W));
    EXPECT_FALSE(view.wasReleased(Key::A));
    EXPECT_FALSE(view.isDown(MouseButton::Left));
    EXPECT_FALSE(view.wasPressed(MouseButton::Left));
}
