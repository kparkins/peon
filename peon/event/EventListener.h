#ifndef PEON_EVENT_LISTENER_H
#define PEON_EVENT_LISTENER_H

namespace Peon {

    template<typename EventType>
    class EventDispatcher;

    template<typename EventType>
    class EventListener : public EventType {

    public:

        EventListener();
        virtual ~EventListener();

    };

}

#include "EventListener.inl"
#endif