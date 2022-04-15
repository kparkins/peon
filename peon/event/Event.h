#ifndef PEON_EVENT_H
#define PEON_EVENT_H

namespace Peon {

typedef size_t EventType;

class BaseEvent {
 public:
  virtual ~BaseEvent();

 protected:
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