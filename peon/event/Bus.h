#ifndef PEON_BUS_H
#define PEON_BUS_H

#include <unordered_map>

#include "Event.h"
#include "Signal.h"
#include "common/Uncopyable.h"

using std::unordered_map;

namespace Peon {
class Bus : public Peon::Uncopyable {
 public:
  ~Bus() {
    for (auto pair : mBus) {
      delete pair.second;
    }
    mBus.clear();
  }

  template <typename E>
  Connection<E> Connect(Receiver<E> func) {
    EventType type = Event<E>::Type();
    if (mBus.find(type) == mBus.end()) {
      mBus[type] = new Signal<E>();
    }
    Signal<E>* signal = static_cast<Signal<E>*>(mBus[type]);
    auto receiver = MakeShared<Receiver<E>>(move(func));
    signal->Connect(receiver);
    return receiver;
  }

  template <typename E, typename O, typename M>
  Connection<E> Connect(O* object, M func) {
    return this->Connect<E>(bind(func, object, std::placeholders::_1));
  }

  template <typename E>
  void Disconnect(Connection<E> connection) {
    EventType type = Event<E>::Type();
    Signal<E>* signal = static_cast<Signal<E>*>(mBus[type]);
    signal->Disconnect(connection);
    connection.reset();
  }

  template <typename E, typename... Args>
  void Emit(Args&&... args) {
    EventType type = Event<E>::Type();
    if (mBus.find(type) == mBus.end()) {
      return;
    }
    E event(forward<Args>(args)...);
    Signal<E>* signal = static_cast<Signal<E>*>(mBus[type]);
    signal->Emit(event);
  }

 protected:
  unordered_map<EventType, BaseSignal*> mBus;
};

}  // namespace Peon
#endif