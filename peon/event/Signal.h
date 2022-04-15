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

class BaseSignal {
 public:
  virtual ~BaseSignal() = default;
};

template <typename E>
class Signal : public BaseSignal {
 public:
  void Connect(const Connection<E> connection) {
    Handle<E> handle = connection;
    mHandles.push_back(handle);
  }

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

 protected:
  vector<Handle<E>> mHandles;
};

}  // namespace Peon
#endif