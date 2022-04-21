#ifndef PEON_SIGNAL_H
#define PEON_SIGNAL_H

#include <functional>
#include <unordered_map>
#include <vector>

#include "common/TypeAliases.h"

using std::function;
using std::unordered_map;
using std::vector;

namespace Peon {

template <typename E>
using Receiver = function<void(const E&)>;

template <typename E>
using Handle = Peon::Weak<Receiver<E>>;

template <typename E>
using Connection = Peon::Shared<Receiver<E>>;

class BaseSignal {
 public:
  virtual ~BaseSignal() = default;
};

template <typename E>
class Signal : public BaseSignal {
 public:
  void Connect(Connection<E> connection);
  void WeakConnect(Connection<E> connection);
  void Disconnect(Connection<E> connection);
  void Emit(const E& event);

 protected:
  unordered_map<Connection<E>, bool> mReceivers;
};

template <typename E>
void Signal<E>::Connect(Connection<E> connection) {
  mReceivers[connection] = true;
}

template <typename E>
void Signal<E>::WeakConnect(Connection<E> connection) {
  mReceivers[connection] = false;
}

template <typename E>
void Signal<E>::Disconnect(Connection<E> connection) {
  mReceivers.erase(connection)
}

template <typename E>
void Signal<E>::Emit(const E& event) {
  auto it = mReceivers.begin();
  while (it != mReceivers.end()) {
    // if the connectee released their handle and
    // didn't specify to keep the connection alive
    if (it->first.use_count() <= 1 && !it->second) {
      it = mReceivers.erase(it);
    } else {
      (*it->first)(event);
      ++it;
    }
  }
}

}  // namespace Peon
#endif