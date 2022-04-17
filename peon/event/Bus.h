#ifndef PEON_BUS_H
#define PEON_BUS_H

#include <unordered_map>

#include "Event.h"
#include "Signal.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"

using std::forward;
using std::unordered_map;

namespace Peon {

class Bus : public Peon::Uncopyable {
 public:
  ~Bus();

  template <typename E>
  Connection<E> Connect(Receiver<E> func);

  template <typename E, typename O, typename M>
  inline Connection<E> Connect(O object, M func);

  template <typename E>
  Connection<E> WeakConnect(Receiver<E> func);

  template <typename E, typename O, typename M>
  inline Connection<E> WeakConnect(O object, M func);

  template <typename E>
  void Disconnect(Connection<E> connection);

  template <typename E, typename... Args>
  void Emit(Args&&... args);

 protected:
  template <typename E>
  inline Signal<E>* EnsureSignal();

  template <typename E>
  inline Signal<E>* GetSignal();

  unordered_map<EventType, BaseSignal*> mBus;
};

template <typename E>
Connection<E> Bus::Connect(Receiver<E> func) {
  Signal<E>* signal = this->EnsureSignal<E>();
  auto receiver = MakeShared<Receiver<E>>(move(func));
  signal->Connect(receiver);
  return receiver;
}

template <typename E, typename O, typename M>
inline Connection<E> Bus::Connect(O object, M func) {
  return this->Connect<E>(bind(func, object, std::placeholders::_1));
}

template <typename E>
Connection<E> Bus::WeakConnect(Receiver<E> func) {
  Signal<E>* signal = this->EnsureSignal<E>();
  auto receiver = MakeShared<Receiver<E>>(move(func));
  signal->WeakConnect(receiver);
  return receiver;
}

template <typename E, typename O, typename M>
inline Connection<E> Bus::WeakConnect(O object, M func) {
  return this->WeakConnect<E>(bind(func, object, std::placeholders::_1));
}

template <typename E>
void Bus::Disconnect(Connection<E> connection) {
  Signal<E>* signal = this->GetSignal<E>();
  if (!signal) {
    return;
  }
  signal->Disconnect(connection);
  connection.reset();
}

template <typename E, typename... Args>
void Bus::Emit(Args&&... args) {
  Signal<E>* signal = this->GetSignal<E>();
  if (!signal) {
    return;
  }
  E event(forward<Args>(args)...);
  signal->Emit(event);
}

template <typename E>
inline Signal<E>* Bus::EnsureSignal() {
  EventType type = Event<E>::Type();
  if (mBus.find(type) == mBus.end()) {
    mBus[type] = new Signal<E>();
  }
  return static_cast<Signal<E>*>(mBus[type]);
}

template <typename E>
inline Signal<E>* Bus::GetSignal() {
  EventType type = Event<E>::Type();
  if (mBus.find(type) == mBus.end()) {
    return nullptr;
  }
  return static_cast<Signal<E>*>(mBus[type]);
}

}  // namespace Peon
#endif