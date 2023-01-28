#ifndef PEON_COMPONENT_H
#define PEON_COMPONENT_H

#include <bitset>
#include <cstdint>

#include "EcsTypes.h"
#include "Pool.h"

namespace Peon {

using std::bitset;

extern uint32_t gLastComponentId;

template <typename C>
class Component {
 public:
  Component() : pool(nullptr), id(INVALID_ENTITY_ID) {}
  explicit Component(Pool* pool, EntityId id) : pool(pool), id(id) {}

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
  EntityId id;
  EntityIndex index;
};

template <typename C>
inline Component<C>::operator bool() const {
  return this->IsValid();
}

template <typename C>
inline bool Component<C>::IsValid() const {
  return GetEntityIndex(id) != INVALID_ENTITY_INDEX &&
         pool->Get(GetEntityIndex(id)) != nullptr;
}

template <typename C>
inline C* Component<C>::operator->() {
  assert(this->IsValid());
  return static_cast<C*>(pool->Get(GetEntityIndex(id)));
}

template <typename C>
inline C* Component<C>::operator->() const {
  assert(this->IsValid());
  return static_cast<C*>(pool->Get(GetEntityIndex(id)));
}

template <typename C>
inline C& Component<C>::operator*() {
  assert(this->IsValid());
  return *static_cast<C*>(pool->Get(GetEntityIndex(id)));
}

template <typename C>
inline const C& Component<C>::operator*() const {
  assert(this->IsValid());
  return *static_cast<C*>(pool->Get(GetEntityIndex(id)));
}

}  // namespace Peon
#endif