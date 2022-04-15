#ifndef PEON_EVENT_LISTENER_H
#define PEON_EVENT_LISTENER_H

#include <unordered_set>

using std::unordered_set;

namespace Peon {

template <typename EventType>
class EventListener {
 public:
  EventListener() = default;
  virtual ~EventListener() = default;
};

}  // namespace Peon

#endif