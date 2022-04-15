#ifndef PEON_KEY_EVENT_LISTENER_H
#define PEON_KEY_EVENT_LISTENER_H

#include "Event.h"
#include "EventListener.h"
#include "input/Key.h"
#include "log/Logger.h"

using Peon::Key;

namespace Peon {

class KeyEvent : public BaseEvent {
 public:
  KeyEvent(Key key, KeyAction action) : key(key), action(action) {}
  Key key;
  KeyAction action;
};

template <>
class EventListener<KeyEvent> {
 public:
  EventListener() { /*PEON_CONNECT(KeyEvent, this); */
  }
  ~EventListener() { /* PEON_DISCONNECT(KeyEvent, this); */
  }

  virtual void OnKeyEvent(const KeyEvent& event) = 0;
};
}  // namespace Peon

#endif