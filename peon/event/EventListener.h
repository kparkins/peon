#ifndef PEON_EVENT_HANDLER_H
#define PEON_EVENT_HANDLER_H

#include <unordered_set>

using std::unordered_set;

namespace Peon {

    typedef unsigned int EventBus;

    template<typename EventType>
    class EventDispatcher;

    template<typename EventType>
    class EventListener {

    public:

        EventListener();
        virtual ~EventListener();

    private:

        unordered_set<EventBus> mEventBuses;

        template<typename EventType>
        friend class EventDispatcher;

    };

}

#include "EventListener.inl"
#endif