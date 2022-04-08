
template <typename EventType>
std::vector<Peon::EventListener<EventType>*>
    Peon::EventDispatcher<EventType>::mListeners;

template <typename EventType>
inline void Peon::EventDispatcher<EventType>::AddListener(
    EventListener<EventType>* handler) {
  assert(handler);
  mListeners.push_back(handler);
}

template <typename EventType>
inline void Peon::EventDispatcher<EventType>::RemoveListener(
    EventListener<EventType>* handler) {
  assert(handler);
  int index = 0;
  for (; index < mListeners.size(); ++index) {
    if (mListeners[index] == handler) {
      mListeners[index] = mListeners[mListeners.size() - 1];
      mListeners.pop_back();
      return;
    }
  }
}

template <typename EventType>
template <typename Function, typename... Args>
inline void Peon::EventDispatcher<EventType>::Event(Function func,
                                                    Args&&... args) {
  for (auto& listener : mListeners) {
    (listener->*func)(forward<Args>(args)...);
  }
}
