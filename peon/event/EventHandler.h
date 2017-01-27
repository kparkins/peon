#ifndef PEON_EVENT_HANDLER_H
#define PEON_EVENT_HANDLER_H

#include <unordered_set>

using std::unordered_set;

namespace Peon {

    typedef unsigned int EventBus;

    template<typename EventType>
    class EventGroup;

    template<typename EventType>
    class EventHandler : public EventType {

    public:

        EventHandler();
        virtual ~EventHandler();

    private:

        unordered_set<EventBus> mEventBuses;

        template<typename EventType>
        friend class EventGroup;

    };

}

#include "EventHandler.inl"
#endif