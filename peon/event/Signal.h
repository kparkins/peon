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
class Cxn {
 public:
  virtual ~Cxn() {}
  virtual bool IsValid() const = 0;
  virtual Connection<E> Get() const = 0;
  virtual bool operator==(const Cxn& rhs) const = 0;
};

template <typename E>
class Transient : public Cxn<E> {
 public:
  Transient(Connection<E> conn) { mHandle = conn; }

  bool IsValid() const override { return !mHandle.expired(); }
  Connection<E> Get() const override { return mHandle.lock(); }

  bool operator==(const Cxn& rhs) const override {
    return mHandle.lock() == rhs.Get();
  }

 protected:
  Handle<E> mHandle;
};

template <typename E>
class Persistent : public Cxn<E> {
 public:
  Persistent(Connection<E> conn) : mConnection(conn) {}

  bool IsValid() const override { return mConnection.get() != 0; }
  Connection<E> Get() const override { return mConnection; }

  bool operator==(const Cxn& rhs) const override {
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
  void Connect(Unique<Cxn<E>> cxn) {
    assert(cxn->IsValid());
    mCxns.push_back(move(cxn));
  }

  void Disconnect(Connection<E> connection) {
    auto pred = [=](Unique<Cxn<E>>& cxn) { return cxn->Get() == connection; };
    this->Remove(pred, mCxns);
  }

  void Emit(const E& event) {
    auto pred = [](Unique<Cxn<E>>& cxn) { return !cxn->IsValid(); };
    this->Remove(pred, mCxns);
    for (auto& cxn : mCxns) {
      auto receiver = cxn->Get();
      if (receiver) {
        (*receiver)(event);
      }
    }
  }

 protected:
  inline void Remove(function<bool(Unique<Cxn<E>>&)> predicate,
                     vector<Unique<Cxn<E>>>& collection) {
    auto indices = remove_if(collection.begin(), collection.end(), predicate);
    collection.erase(indices, collection.end());
  }

  vector<Unique<Cxn<E>>> mCxns;
};

}  // namespace Peon
#endif