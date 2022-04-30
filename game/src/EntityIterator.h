#ifndef GAME_ENTITY_ITERATOR_H
#define GAME_ENTITY_ITERATOR_H

#include <vector>
#include <cstddef>

#include "Entity.h"

using std::vector;

class EntityIterator {
 public:
  EntityIterator(size_t index, vector<Entity*> entities,
                 ComponentMask included);

  Entity* operator*();
  const Entity* operator*() const;

  Entity* operator->();
  const Entity* operator->() const;

  EntityIterator& operator++();
  EntityIterator& operator--();
  EntityIterator operator++(int);
  EntityIterator operator--(int);
  EntityIterator& operator+=(int n);
  EntityIterator& operator-=(int n);
  EntityIterator operator+(int n) const;
  EntityIterator operator-(int n) const;

  size_t operator-(EntityIterator const& r) const;
  bool operator<(EntityIterator const& r) const;
  bool operator<=(EntityIterator const& r) const;
  bool operator>(EntityIterator const& r) const;
  bool operator>=(EntityIterator const& r) const;
  bool operator!=(const EntityIterator& r) const;
  bool operator==(const EntityIterator& r) const;

 private:
  size_t index;
  vector<Entity*> entities;
  ComponentMask included;
};

#endif