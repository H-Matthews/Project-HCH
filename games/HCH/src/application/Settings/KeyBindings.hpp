#pragma once

#include "application/Data/PlayerAction.hpp"

#include "core/Event.hpp"

#include <map>

namespace Application {
/**
 * This class maintains the data mapping between a Key on the Keyboard and a Player Action
 */

class KeyBindings {
  public:
    KeyBindings();

    void assignKey(Actions::Player action, Core::Key key);
    Actions::Player getAssignedAction(Core::Key key) const;

    const std::map<Core::Key, Actions::Player>& getKeyBindings();
    static bool isRealTimeAction(Actions::Player action);

  private:
    std::map<Core::Key, Actions::Player> mKeyBindings;
};
} // namespace Application