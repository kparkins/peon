#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <cassert>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "common/Uncopyable.h"

class SceneView;

using std::forward;
using std::vector;

class Scene : public Peon::Uncopyable {
 public:
  explicit Scene() = default;
  virtual ~Scene();

  Entity* CreateEntity();
  void DestroyEntity(Entity* entity);

  template <typename T>
  Component<T> AddComponent(Entity* entity);

  template <typename T>
  Component<T> GetComponent(Entity* entity);

  template <typename T>
  void RemoveComponent(Entity* entity);

 protected:
  template <typename T>
  T* Access(Entity* entity);

  template <typename T>
  friend class Component;
  friend class SceneView;
  vector<EntityIndex> mFreeList;
  vector<Entity*> mEntities;
  vector<Pool*> mComponents;
};

template <typename T>
T* Scene::Access(Entity* entity) {
  ComponentId componentId = Component<T>::Id();
  Pool* pool = mComponents[componentId];
  assert(entity->IsValid());
  assert(componentId < mComponents.size());
  return static_cast<T*>(pool->Get(entity->GetIndex()));
}

template <typename T>
Component<T> Scene::AddComponent(Entity* entity) {
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
  pool->Allocate(index);
  entity->Add(componentId);
  return Component<T>(this, entity);
}

template <typename T>
Component<T> Scene::GetComponent(Entity* entity) {
  EntityIndex index = entity->GetIndex();
  if (!entity->HasComponent<T>()) {
    return nullptr;
  }
  assert(entity->IsValid());
  assert(componentId < mComponents.size());
  return Component<T>(this, entity);
}

template <typename T>
void Scene::RemoveComponent(Entity* entity) {
  EntityIndex index = entity->GetIndex();
  ComponentId componentId = Component<T>::Id();
  assert(entity->IsValid());
  assert(entity->HasComponent<T>());
  assert(componentId < mComponents.size());
  Pool* pool = mComponents[componentId];
  pool->Free(index);
  entity->Remove(componentId);
}

#endif