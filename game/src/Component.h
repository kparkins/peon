#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <bitset>
#include <cstdint>

using std::bitset;

typedef uint64_t ComponentId;
const uint32_t MAX_COMPONENTS = sizeof(ComponentId) * 8;
extern uint64_t gLastComponentId;

typedef bitset<MAX_COMPONENTS> ComponentMask;

class Scene;
class Entity;

template <typename C>
class Component {
 public:
  Component() : scene(nullptr), entity(nullptr) {}

  inline operator bool() const;
  inline bool IsValid() const;
  inline C* operator->();
  inline C* operator->() const;
  inline C& operator*();
  inline const C& operator*() const;

  inline Entity* GetEntity() const { return entity; }
  inline Scene* GetScene() const { return scene; }

  static ComponentId Id() {
    static ComponentId componentId = gLastComponentId++;
    return componentId;
  }

 protected:
  explicit Component(Scene* scene, Entity* entity)
      : scene(scene), entity(entity) {}

  friend class Entity;
  friend class Scene;
  Scene* scene;
  Entity* entity;
};

template <typename C>
inline Component<C>::operator bool() const {
  return this->IsValid();
}

template <typename C>
inline bool Component<C>::IsValid() const {
  return scene && entity && entity->IsValid() && entity->HasComponent<C>();
}

template <typename C>
inline C* Component<C>::operator->() {
  assert(this->IsValid());
  return scene->Access<C>(entity);
}

template <typename C>
inline C* Component<C>::operator->() const {
  assert(this->IsValid());
  return scene->Access<C>(entity);
}

template <typename C>
inline C& Component<C>::operator*() {
  assert(this->IsValid());
  return *scene->Access<C>(entity);
}

template <typename C>
inline const C& Component<C>::operator*() const {
  assert(this->IsValid());
  return *scene->Access<C>(entity);
}

#endif