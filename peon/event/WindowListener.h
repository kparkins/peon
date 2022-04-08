#ifndef PEON_WINDOW_LISTENER_H
#define PEON_WINDOW_LISTENER_H

#include "Event.h"
#include "EventListener.h"

namespace Peon {
class WindowEvent : public Event {
};

template <>
class EventListener<WindowEvent> {
 public:
  virtual void WindowResized() {}
  virtual void WindowMoved() {}
  virtual void WindowDamaged() {}
  virtual void WindowClosed() {}
  virtual void WindowFocused() {}
  virtual void WindowMinimized() {}
  virtual void WindowFramebufferChanged() {}
};

}  // namespace Peon

#endif