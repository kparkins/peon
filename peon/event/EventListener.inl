template<typename EventType>
Peon::EventListener<EventType>::EventListener() {
    EventDispatcher<EvenType>::mBuses[0].insert(this);
    mEventBuses.insert(0);
}

template<typename EventType>
Peon::EventListener<EventType>::~EventListener() {
    for (auto bus : mEventBuses) {
        EventDispatcher<EventType>::mBuses[bus].erase(this);
    }
}
