#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <cassert>
#include <tuple>
#include <vector>

#include "Component.h"
#include "EcsTypes.h"
#include "EntityView.h"
#include "common/Uncopyable.h"
#include "glm/glm.hpp"

using glm::mat4;
using glm::vec3;
using glm::vec4;
using std::forward;
using std::tuple;
using std::vector;

class Scene;

class Entity : public Peon::Uncopyable {
 public:
  inline bool IsValid() const;

  inline EntityId GetId() const;
  inline EntityIndex GetIndex() const;
  inline EntityVersion GetVersion() const;

  template <typename T, typename... Args>
  inline Component<T> AddComponent(Args&&... args);

  template <typename T>
  inline Component<T> GetComponent();

  template <typename T>
  inline void RemoveComponent();

  template <typename T>
  inline bool HasComponent() const;

  template <typename... Components>
  inline bool HasComponents() const;
  inline bool HasComponents(ComponentMask mask) const;

  inline ComponentMask GetComponents() const;

 protected:
  explicit Entity();
  virtual ~Entity();
  explicit Entity(EntityIndex index, EntityVersion version);
  void SetId(EntityIndex index, EntityVersion version);

  inline void Add(ComponentId id);
  inline void Remove(ComponentId id);

  friend class Scene;

  ComponentMask mComponents;
  Scene* mScene;
  EntityId mId;
};

class Scene : public Peon::Uncopyable {
 public:
  explicit Scene() = default;
  ~Scene();

  Entity* CreateEntity();
  void DestroyEntity(Entity* entity);

  template <typename T, typename... Args>
  Component<T> AddComponent(Entity* entity, Args&&... args);

  template <typename T>
  Component<T> GetComponent(Entity* entity);

  template <typename T>
  void RemoveComponent(Entity* entity);

  template <typename... Included>
  EntityView<Included...> EntitiesWith();

 protected:
  vector<EntityIndex> mFreeList;
  vector<Entity*> mEntities;
  vector<Pool*> mComponents;
  vector<EntitySlot> entities;
};

template <typename... Included>
EntityView<Included...> Scene::EntitiesWith() {
  return EntityView<Included...>(this->mEntities);
}

template <typename T, typename... Args>
Component<T> Scene::AddComponent(Entity* entity, Args&&... args) {
  assert(entity->IsValid());
  ComponentId componentId = Component<T>::Id();
  if (componentId >= mComponents.size()) {
    mComponents.resize(componentId + 1, nullptr);
  }
  EntityIndex index = entity->GetIndex();
  if (mComponents[componentId] == nullptr) {
    Pool* pool = new PackedPool<T>();
    mComponents[componentId] = pool;
  }
  Pool* pool = mComponents[componentId];
  assert(pool->Get(index) == nullptr);
  void* memory = pool->Allocate(index);
  new (memory) T(forward<Args>(args)...);
  entity->Add(componentId);
  return Component<T>(pool, entity->GetId());
}

template <typename T>
Component<T> Scene::GetComponent(Entity* entity) {
  if (!entity->HasComponent<T>()) {
    return Component<T>();
  }
  ComponentId componentId = Component<T>::Id();
  assert(entity->IsValid());
  assert(componentId < mComponents.size());
  Pool* pool = mComponents[componentId];
  return Component<T>(pool, entity->GetId());
}

template <typename T>
void Scene::RemoveComponent(Entity* entity) {
  EntityIndex index = entity->GetIndex();
  ComponentId componentId = Component<T>::Id();
  assert(componentId != Component<Transform>::Id());
  assert(entity->IsValid());
  assert(entity->HasComponent<T>());
  assert(componentId < mComponents.size());
  Pool* pool = mComponents[componentId];
  pool->Destroy(index);
  pool->Free(index);
  entity->Remove(componentId);
}

inline EntityVersion Entity::GetVersion() const {
  return static_cast<EntityVersion>(mId & numeric_limits<uint32_t>::max());
}

inline EntityIndex Entity::GetIndex() const { return mId >> 32; }
inline EntityId Entity::GetId() const { return mId; }
inline bool Entity::IsValid() const {
  return (mId >> 32) != INVALID_ENTITY_INDEX;
}

template <typename T, typename... Args>
inline Component<T> Entity::AddComponent(Args&&... args) {
  if (!this->IsValid()) {
    return Component<T>(nullptr, INVALID_ENTITY_INDEX);
  }
  return mScene->AddComponent<T>(this, forward<Args>(args)...);
}

template <typename T>
inline Component<T> Entity::GetComponent() {
  if (!this->IsValid()) {
    return Component<T>(nullptr, INVALID_ENTITY_INDEX);
  }
  return mScene->GetComponent<T>(this);
}

template <typename T>
inline void Entity::RemoveComponent() {
  if (this->IsValid() && mScene->GetComponent<T>(this)) {
    mScene->RemoveComponent<T>(this);
  }
}

template <typename T>
inline bool Entity::HasComponent() const {
  ComponentId id = Component<T>::Id();
  return this->mComponents.test(id);
}

template <typename... Components>
inline bool Entity::HasComponents() const {
  ComponentMask mask;
  ComponentId componentIds[] = {0, Component<Components>::Id()...};
  bool result = true;
  for (int i = 1; i < (sizeof...(Components) + 1); ++i) {
    result &= mComponents.test(componentIds[i]);
  }
  return result;
}

inline bool Entity::HasComponents(ComponentMask mask) const {
  return (mask & mComponents) == mask;
}

inline ComponentMask Entity::GetComponents() const { return this->mComponents; }

inline void Entity::Add(ComponentId id) { this->mComponents.set(id); }
inline void Entity::Remove(ComponentId id) { this->mComponents.reset(id); }

#endif