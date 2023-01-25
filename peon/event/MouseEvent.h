#ifndef PEON_MOUSE_EVENT_H
#define PEON_MOUSE_EVENT_H

#include "Event.h"

namespace Peon {

class MouseMove : public Event<MouseMove> {
 public:
  explicit MouseMove(const MouseMove& other) : x(other.x), y(other.y) {}
  explicit MouseMove(float x, float y) : x(x), y(y) {}

  float x;
  float y;
};

}  // namespace Peon
#endif