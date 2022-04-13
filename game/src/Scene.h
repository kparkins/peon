#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <cassert>
#include <vector>

#include "Component.h"
#include "Entity.h"

using std::vector;

class Scene {
 public:
  Scene() {}
  virtual ~Scene() {}

  EntityId CreateEntity();
  void DestroyEntity(EntityId id);

  template <typename T>
  T* AddComponent(EntityId id);

  template <typename T>
  T* GetComponent(EntityId id);

  template <typename T>
  void RemoveComponent(EntityId id);

 protected:
  vector<EntityIndex> mFreeList;
  vector<EntityId> mEntities;
  vector<Pool*> mComponents;
};

template <typename T>
T* Scene::AddComponent(EntityId id) {
  assert(IsEntityValid(id));
  ComponentId componentId = GetComponentId<T>();
  if (componentId >= mComponents.size()) {
    mComponents.resize(componentId + 1, nullptr);
  }
  EntityIndex index = GetEntityIndex(id);
  if (mComponents[componentId] == nullptr) {
    Pool* pool = new PackedPool<T>();
    mComponents[componentId] = pool;
  }
  Pool* pool = mComponents[componentId];
  assert(pool->Get(index) == nullptr);
  T* component = new (mComponents[componentId]->Allocate(index)) T();
  return component;
}

template <typename T>
T* Scene::GetComponent(EntityId id) {
  EntityIndex index = GetEntityIndex(id);
  ComponentId componentId = GetComponentId<T>();
  assert(IsEntityValid(id));
  assert(componentId < mComponents.size());
  Pool* pool = mComponents[componentId];
  return static_cast<T*>(pool->Get(index));
}

template <typename T>
void Scene::RemoveComponent(EntityId id) {
  EntityIndex index = GetEntityIndex(id);
  ComponentId componentId = GetComponentId<T>();
  assert(IsEntityValid(id));
  assert(componentId < mComponents.size());
  Pool* pool = mComponents[componentId];
  pool->Delete(index);
}

#endif