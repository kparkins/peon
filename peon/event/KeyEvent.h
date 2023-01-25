#ifndef PEON_KEY_EVENT_H
#define PEON_KEY_EVENT_H

#include "Event.h"
#include "input/Key.h"
#include "log/Logger.h"

using Peon::Key;

namespace Peon {

class KeyEvent : public Event<KeyEvent> {
 public:
  KeyEvent(int key, int action)
      : key(static_cast<Peon::Key>(key)),
        action(static_cast<Peon::KeyAction>(action)) {}
  KeyEvent(Key key, KeyAction action) : key(key), action(action) {}
  Key key;
  KeyAction action;
};

}  // namespace Peon

#endif