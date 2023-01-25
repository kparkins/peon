#include "PoolIterator.h"

tuple<size_t, void*> operator*() override;
const tuple<size_t, void*> operator*() const override;

tuple<size_t, void*> operator->() override;
const tuple<size_t, void*> operator->() const override;

PackedPoolIterator& operator++() override;
PackedPoolIterator& operator--() override;
PackedPoolIterator operator++(int) override;
PackedPoolIterator operator--(int) override;
PackedPoolIterator& operator+=(int n) override;
PackedPoolIterator& operator-=(int n) override;
PackedPoolIterator operator+(int n) const override;
PackedPoolIterator operator-(int n) const override;

size_t operator-(PackedPoolIterator const& r) const override;
bool operator<(PackedPoolIterator const& r) const override;
bool operator<=(PackedPoolIterator const& r) const override;
bool operator>(PackedPoolIterator const& r) const override;
bool operator>=(PackedPoolIterator const& r) const override;
bool operator!=(const PackedPoolIterator& r) const override;
bool operator==(const PackedPoolIterator& r) const override;
