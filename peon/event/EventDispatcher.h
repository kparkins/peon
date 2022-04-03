#ifndef PEON_EVENT_GROUP_H
#define PEON_EVENT_GROUP_H

#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>

#include "EventListener.h"

using std::forward;
using std::unordered_set;
using std::unordered_map;

namespace Peon {

    template<typename EventType>
    class EventDispatcher {

    public:

        inline static void AddListener(EventBus bus, EventListener<EventType>* handler);
        inline static void RemoveListener(EventBus bus, EventListener<EventType>* handler);

        template<typename Function, typename... Args>
        inline static void Event(EventBus bus, Function func, Args&&... args);

    protected:

        static unordered_map<EventBus, unordered_set<EventListener<EventType>*>> mBuses;

        template<typename EventType>
        friend class EventListener;

    };

#define PEON_BROADCAST(Group, Func, ...) { EventDispatcher<Group>::Event(0, &Group::Func, __VA_ARGS__); } 
#define PEON_EVENT(Group, Bus, Func, ...) { EventDispatcher<Group>::Event(Bus, &Group::Func, __VA_ARGS__); }
#define PEON_CONNECT(Group, Bus, Handler) { EventDispatcher<Group>::AddListener(Bus, Handler); }
#define PEON_DISCONNECT(Group, Bus, Handler) { EventDispatcher<Group>::RemoveListener(Bus, Handler); }

}

#include "EventDispatcher.inl"
#endif