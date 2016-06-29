
template<typename EventType>
Peon::EventListener<EventType>::EventListener() {
    EventDispatcher<EventType>::AddListener(this);
}

template<typename EventType>
Peon::EventListener<EventType>::~EventListener() {
    EventDispatcher<EventType>::RemoveListener(this);
}
