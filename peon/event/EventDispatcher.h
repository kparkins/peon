#ifndef PEON_EVENT_DISPATCHER_H
#define PEON_EVENT_DISPATCHER_H

#include <vector>
#include <utility>

#include "EventListener.h"

using std::vector;
using std::forward;

namespace Peon {

    template<typename EventType>
    class EventDispatcher {

    public:

        inline static void AddListener(EventListener<EventType>* listener);
        inline static void RemoveListener(EventListener<EventType>* listener);
        
        template<typename Function, typename... Args>
        inline static void Event(Function func, Args&&... args);

    protected:

        static vector<EventListener<EventType>*> mListeners;

    };

#define PEON_EVENT(Type, Func, ...) { EventDispatcher<Type>::Event(&Type::Func, __VA_ARGS__); } 
#define PEON_EVENT_SUBSCRIBE(Type, listener) { EventDispatcher<Type>::AddListener(listener); } 
#define PEON_EVENT_UNSUBSCRIBE(Type, listener) { EventDispatcher<Type>::RemoveListener(listener); }

}

#include "EventDispatcher.inl"
#endif