#ifndef PEON_MOUSE_EVENT_H
#define PEON_MOUSE_EVENT_H

#include "Event.h"
#include "EventListener.h"

namespace Peon {

class MouseMove : public Event {
 public:
  explicit MouseMove(const MouseMove& other) : x(other.x), y(other.y) {}
  explicit MouseMove(float x, float y) : x(x), y(y) {}

  float x;
  float y;
};

template <>
class EventListener<MouseMove> {
 public:
  EventListener() { PEON_CONNECT(MouseMove, this); }
  ~EventListener() { PEON_DISCONNECT(MouseMove, this); }

  virtual void OnMouseMove(const MouseMove& event) = 0;
};

}  // namespace Peon
#endif