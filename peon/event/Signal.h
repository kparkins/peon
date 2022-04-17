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

template <typename E>
class ConnectionState {
 public:
  virtual ~ConnectionState() {}
  virtual bool IsValid() const = 0;
  virtual Connection<E> Get() const = 0;
  virtual bool operator==(const ConnectionState& rhs) const = 0;
};

template <typename E>
class Transient : public ConnectionState<E> {
 public:
  Transient(Connection<E> conn) { mHandle = conn; }
  bool IsValid() const override { return !mHandle.expired(); }
  Connection<E> Get() const override { return mHandle.lock(); }
  bool operator==(const ConnectionState& rhs) const override {
    return mHandle.lock() == rhs.Get();
  }

 protected:
  Handle<E> mHandle;
};

template <typename E>
class Persistent : public ConnectionState<E> {
 public:
  Persistent(Connection<E> conn) : mConnection(conn) {}
  bool IsValid() const override { return mConnection.get() != 0; }
  Connection<E> Get() const override { return mConnection; }
  bool operator==(const ConnectionState& rhs) const override {
    return mConnection == rhs.Get();
  }

 protected:
  Connection<E> mConnection;
};

class BaseSignal {
 public:
  virtual ~BaseSignal() = default;
};

template <typename E>
class Signal : public BaseSignal {
 public:
  using ConnectionState = Unique<ConnectionState<E>>;

  void Connect(Connection<E> connection);
  void WeakConnect(Connection<E> connection);
  void Disconnect(Connection<E> connection);
  void Emit(const E& event);

 protected:
  inline void Remove(function<bool(ConnectionState&)> predicate,
                     vector<ConnectionState>& collection);

  vector<ConnectionState> mConnectionStates;
};

template <typename E>
void Signal<E>::Connect(Connection<E> connection) {
  auto connectionState = MakeUnique<Persistent<E>>(move(connection));
  assert(connectionState->IsValid());
  mConnectionStates.push_back(move(connectionState));
}

template <typename E>
void Signal<E>::WeakConnect(Connection<E> connection) {
  auto connectionState = MakeUnique<Transient<E>>(move(connection));
  assert(connectionState->IsValid());
  mConnectionStates.push_back(move(connectionState));
}

template <typename E>
void Signal<E>::Disconnect(Connection<E> connection) {
  auto pred = [=](ConnectionState& state) {
    return state->Get() == connection;
  };
  this->Remove(pred, mConnectionStates);
}

template <typename E>
void Signal<E>::Emit(const E& event) {
  auto pred = [](ConnectionState& state) { return !state->IsValid(); };
  this->Remove(pred, mConnectionStates);
  for (auto& connectionState : mConnectionStates) {
    auto receiver = connectionState->Get();
    if (receiver) {
      (*receiver)(event);
    }
  }
}

template <typename E>
inline void Signal<E>::Remove(function<bool(ConnectionState&)> predicate,
                              vector<ConnectionState>& collection) {
  auto indices = remove_if(collection.begin(), collection.end(), predicate);
  collection.erase(indices, collection.end());
}

}  // namespace Peon
#endif