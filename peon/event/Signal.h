#ifndef PEON_SIGNAL_H
#define PEON_SIGNAL_H

#include <functional>
#include <vector>

#include "common/TypeAliases.h"

using std::function;
using std::vector;

namespace Peon {
template <typename E>
using Receiver = function<void(const E&)>;

template <typename E>
using Handle = Peon::Weak<Receiver<E>>;

template <typename E>
using Connection = Peon::Shared<Receiver<E>>;

class ISignal {
 public:
  virtual ~ISignal() = default;
};

template <typename E, template <class E> class ConnectionPolicy>
class SignalBase : public ISignal {
 public:
  virtual void Connect(const Connection<E> connection) {
    ConnectionPolicy<E> handle = connection;
    mHandles.push_back(handle);
  }

 protected:
  vector<ConnectionPolicy<E>> mHandles;
};

template <typename E, template <class E> class ConnectionPolicy>
class Signal : public SignalBase<E, ConnectionPolicy> {
 public:
  void Disconnect(Connection<E> connection);
  void Emit(const E& event);
};

template <typename E>
class Signal<E, Handle> : public SignalBase<E, Handle> {
 public:
  void Disconnect(Connection<E> connection) {
    auto pred = [=](Handle<E> handle) { return handle.lock() == connection; };
    auto indices = remove_if(mHandles.begin(), mHandles.end(), pred);
    mHandles.erase(indices, mHandles.end());
  }

  void Emit(const E& event) {
    auto pred = [=](Handle<E> handle) { return handle.expired(); };
    auto indices = remove_if(mHandles.begin(), mHandles.end(), pred);
    mHandles.erase(indices, mHandles.end());

    for (auto handle : mHandles) {
      auto funcPointer = handle.lock();
      if (funcPointer) {
        (*funcPointer)(event);
      }
    }
  }
};

template <typename E>
class Signal<E, Connection> : public SignalBase<E, Connection> {
 public:
  void Disconnect(Connection<E> connection) {
    auto pred = [=](Connection<E> conn) { return conn == connection; };
    auto indices = remove_if(mHandles.begin(), mHandles.end(), pred);
    mHandles.erase(indices, mHandles.end());
  }

  void Emit(const E& event) {
    auto pred = [=](Connection<E> conn) { return conn.get() == 0; };
    auto indices = remove_if(mHandles.begin(), mHandles.end(), pred);
    mHandles.erase(indices, mHandles.end());
    for (auto handle : mHandles) {
      if (handle) {
        (*handle)(event);
      }
    }
  }
};

}  // namespace Peon
#endif