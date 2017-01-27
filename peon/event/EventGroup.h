#ifndef PEON_EVENT_GROUP_H
#define PEON_EVENT_GROUP_H

#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>

#include "EventHandler.h"

using std::forward;
using std::unordered_set;
using std::unordered_map;

namespace Peon {

    template<typename EventType>
    class EventGroup {

    public:

        inline static void AddHandler(EventBus bus, EventHandler<EventType>* handler);
        inline static void RemoveHandler(EventBus bus, EventHandler<EventType>* handler);

        template<typename Function, typename... Args>
        inline static void Event(EventBus bus, Function func, Args&&... args);

    protected:

        static unordered_map<EventBus, unordered_set<EventHandler<EventType>*>> mBuses;

        template<typename EventType>
        friend class EventHandler;

    };

#define PEON_BROADCAST(Group, Func, ...) { EventGroup<Group>::Event(0, &Group::Func, __VA_ARGS__); } 
#define PEON_EVENT(Group, Bus, Func, ...) { EventGroup<Group>::Event(Bus, &Group::Func, __VA_ARGS__); }
#define PEON_CONNECT(Group, Bus, Handler) { EventGroup<Group>::AddHandler(Bus, Handler); }
#define PEON_DISCONNECT(Group, Bus, Handler) { EventGroup<Group>::RemoveHandler(Bus, Handler); }

}

#include "EventGroup.inl"
#endif