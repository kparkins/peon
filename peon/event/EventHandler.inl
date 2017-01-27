template<typename EventType>
Peon::EventHandler<EventType>::EventHandler() {

}

template<typename EventType>
Peon::EventHandler<EventType>::~EventHandler() {
    for (auto bus : mEventBuses) {
        EventGroup<EventType>::mBuses.erase(bus);
    }
}
