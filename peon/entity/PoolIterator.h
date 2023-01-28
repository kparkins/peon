#ifndef PEON_POOL_ITERATOR_H
#define PEON_POOL_ITERATOR_H

#include <tuple>

using std::tuple;

namespace Peon {

class PoolIterator {
 public:
  virtual tuple<size_t, void*> operator*() = 0;
  virtual const tuple<size_t, void*> operator*() const = 0;

  virtual tuple<size_t, void*> operator->() = 0;
  virtual const tuple<size_t, void*> operator->() const = 0;

  virtual PoolIterator& operator++() = 0;
  virtual PoolIterator& operator--() = 0;
  virtual PoolIterator operator++(int) = 0;
  virtual PoolIterator operator--(int) = 0;
  virtual PoolIterator& operator+=(int n) = 0;
  virtual PoolIterator& operator-=(int n) = 0;
  virtual PoolIterator operator+(int n) const = 0;
  virtual PoolIterator operator-(int n) const = 0;

  virtual size_t operator-(PoolIterator const& r) const = 0;
  virtual bool operator<(PoolIterator const& r) const = 0;
  virtual bool operator<=(PoolIterator const& r) const = 0;
  virtual bool operator>(PoolIterator const& r) const = 0;
  virtual bool operator>=(PoolIterator const& r) const = 0;
  virtual bool operator!=(const PoolIterator& r) const = 0;
  virtual bool operator==(const PoolIterator& r) const = 0;
};

}  // namespace Peon

#endif