
template<typename EventType>
unordered_map<Peon::EventBus, unordered_set<Peon::EventListener<EventType>*>> Peon::EventDispatcher<EventType>::mBuses;

template<typename EventType>
inline void Peon::EventDispatcher<EventType>::AddListener(EventBus bus, EventListener<EventType>* handler) {
    assert(handler);
    mBuses[bus].insert(handler);
    handler->mEventBuses.insert(bus);
}

template<typename EventType>
inline void Peon::EventDispatcher<EventType>::RemoveListener(EventBus bus, EventListener<EventType>* handler) {
    assert(handler);
    if (mBuses.find(bus) == mBuses.end()) {
        LOG_ERROR("Attempt to remove handler from bus that it is not a member of.");
        return; 
    }
    mBuses[bus].erase(handler);
    handler->mEventBuses.erase(bus);
}

template<typename EventType>
template<typename Function, typename... Args>
inline void Peon::EventDispatcher<EventType>::Event(EventBus bus, Function func, Args&&... args) {
    auto & handlers = mBuses[bus];
    for (auto & handler : handlers) {
        (handler->*func)(forward<Args>(args)...);
    }
}