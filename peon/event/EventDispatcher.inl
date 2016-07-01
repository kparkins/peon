
template<typename EventType>
vector<Peon::EventListener<EventType>*> Peon::EventDispatcher<EventType>::mListeners;

template<typename EventType>
inline void Peon::EventDispatcher<EventType>::AddListener(EventListener<EventType>* listener) {
    mListeners.push_back(listener);
}

template<typename EventType>
inline void Peon::EventDispatcher<EventType>::RemoveListener(EventListener<EventType>* listener) {
    
}

template<typename EventType>
template<typename Function, typename ...Args>
inline void Peon::EventDispatcher<EventType>::Event(Function func, Args&&... args) {
    for (auto & listener : mListeners) {
        (listener->*func)(forward<Args>(args)...);
    }
}
