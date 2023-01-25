#ifndef GAME_POOL_ITERATOR_H
#define GAME_POOL_ITERATOR_H

#include <tuple>

using std::tuple;

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

class PackedPoolIterator : public PoolIterator {
 public:
  tuple<size_t, void*> operator*() override;
  const tuple<size_t, void*> operator*() const override;

  tuple<size_t, void*> operator->() override;
  const tuple<size_t, void*> operator->() const override;

  PoolIterator& operator++() override;
  PoolIterator& operator--() override;
  PoolIterator operator++(int) override;
  PoolIterator operator--(int) override;
  PoolIterator& operator+=(int n) override;
  PoolIterator& operator-=(int n) override;
  PoolIterator operator+(int n) const override;
  PoolIterator operator-(int n) const override;

  size_t operator-(PoolIterator const& r) const override;
  bool operator<(PoolIterator const& r) const override;
  bool operator<=(PoolIterator const& r) const override;
  bool operator>(PoolIterator const& r) const override;
  bool operator>=(PoolIterator const& r) const override;
  bool operator!=(const PoolIterator& r) const override;
  bool operator==(const PoolIterator& r) const override;
};

#endif