#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <bitset>
#include <cstdint>
#include "EcsConstants.h"
#include "Pool.h"

using std::bitset;

extern uint64_t gLastComponentId;

template <typename C>
class Component {
 public:
  Component() : pool(nullptr), index(INVALID_ENTITY_INDEX) {}
  explicit Component(Pool* pool, EntityIndex index)
      : pool(pool), index(index) {}

  inline operator bool() const;
  inline bool IsValid() const;
  inline C* operator->();
  inline C* operator->() const;
  inline C& operator*();
  inline const C& operator*() const;

  static ComponentId Id() {
    static ComponentId componentId = gLastComponentId++;
    return componentId;
  }

 protected:
  Pool* pool;
  EntityIndex index;
};

template <typename C>
inline Component<C>::operator bool() const {
  return this->IsValid();
}

template <typename C>
inline bool Component<C>::IsValid() const {

  return index != INVALID_ENTITY_INDEX && pool->Get(index) != nullptr;
}

template <typename C>
inline C* Component<C>::operator->() {
  assert(this->IsValid());
  return static_cast<C*>(pool->Get(index));
}

template <typename C>
inline C* Component<C>::operator->() const {
  assert(this->IsValid());
  return static_cast<C*>(pool->Get(index));
}

template <typename C>
inline C& Component<C>::operator*() {
  assert(this->IsValid());
  return *static_cast<C*>(pool->Get(index));
}

template <typename C>
inline const C& Component<C>::operator*() const {
  assert(this->IsValid());
  return *static_cast<C*>(pool->Get(index));
}

#endif