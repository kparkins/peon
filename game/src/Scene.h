#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <cassert>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "common/Uncopyable.h"
#include "glm/glm.hpp"

class SceneView;

using glm::mat4;
using std::forward;
using std::vector;

struct Transform {
  mat4 matrix;
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

  template <typename T>
  vector<Entity*> GetEntitiesWith();

 protected:
  template <typename T>
  inline T* Access(Entity* entity);

  template <typename T>
  friend class Component;
  friend class SceneView;
  vector<EntityIndex> mFreeList;
  vector<Entity*> mEntities;
  vector<Pool*> mComponents;
};

template <typename T>
vector<Entity*> Scene::GetEntitiesWith() {
  vector<Entity*> result;
  ComponentId id = Component<T>::Id();
  if (id >= mComponents.size()) {
    return result;
  }
  for (auto entity : mEntities) {
    if (entity->HasComponent<T>()) {
      result.push_back(entity);
    }
  }
  return result;
}

template <typename T>
inline T* Scene::Access(Entity* entity) {
  ComponentId componentId = Component<T>::Id();
  Pool* pool = mComponents[componentId];
  assert(entity->IsValid());
  assert(componentId < mComponents.size());
  return static_cast<T*>(pool->Get(entity->GetIndex()));
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
  return Component<T>(this, entity);
}

template <typename T>
Component<T> Scene::GetComponent(Entity* entity) {
  if (!entity->HasComponent<T>()) {
    return Component<T>();
  }
  ComponentId componentId = Component<T>::Id();
  assert(entity->IsValid());
  assert(componentId < mComponents.size());
  return Component<T>(this, entity);
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

#endif