#ifndef PEON_EVENT_H
#define PEON_EVENT_H

#include <cstddef>

namespace Peon {

typedef size_t EventType;

class BaseEvent {
 public:
  virtual ~BaseEvent();

 protected:
  BaseEvent() = default;
  BaseEvent(const BaseEvent& b) = default;
  BaseEvent(BaseEvent&& e) = default;
  BaseEvent& operator=(const BaseEvent& e) = default;
  BaseEvent& operator=(BaseEvent&& e) = default;

  static EventType mTypeCounter;
};

template <typename T>
class Event : public BaseEvent {
 public:
  static EventType Type() {
    static EventType eventType = mTypeCounter++;
    return eventType;
  }
};

}  // namespace Peon

#endif