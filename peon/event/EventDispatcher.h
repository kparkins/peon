#ifndef PEON_EVENT_GROUP_H
#define PEON_EVENT_GROUP_H

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "EventListener.h"

using std::bind;
using std::forward;
using std::function;
using std::string;
using std::unordered_map;
using std::vector;

namespace Peon {

template <typename EventType>
class EventDispatcher {
 public:
  inline static void AddListener(EventListener<EventType>* handler);
  inline static void RemoveListener(EventListener<EventType>* handler);

  template <typename Function, typename... Args>
  inline static void Event(Function func, Args&&... args);

 protected:
  // optimize for fast traversal
  static vector<EventListener<EventType>*> mListeners;

  template <typename EventType>
  friend class EventListener;
};

#define PEON_EVENT(Group, Func, ...) \
  { EventDispatcher<Group>::Event(&EventListener<Group>::Func, __VA_ARGS__); }

#define PEON_CONNECT(Group, Handler) \
  { EventDispatcher<Group>::AddListener(Handler); }

#define PEON_DISCONNECT(Group, Handler) \
  { EventDispatcher<Group>::RemoveListener(Handler); }

}  // namespace Peon

#include "EventDispatcher.inl"
#endif