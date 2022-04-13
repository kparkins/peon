#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <stdint.h>

#include <bitset>
#include <glm/glm.hpp>
#include <vector>

#include "Pool.h"

using glm::vec3;
using std::bitset;
using std::size_t;
using std::vector;

typedef uint64_t EntityId;
typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
const uint32_t MAX_COMPONENTS = 32;
typedef bitset<MAX_COMPONENTS> ComponentMask;

typedef uint64_t ComponentId;
extern uint64_t gLastComponentId;

template <class T>
ComponentId GetComponentId() {
  static ComponentId componentId = gLastComponentId++;
  return componentId;
}

inline EntityId CreateEntityId(EntityIndex index, EntityVersion version) {
  return (static_cast<EntityId>(index) << 32) | static_cast<EntityId>(version);
}

inline EntityIndex GetEntityIndex(EntityId id) { return id >> 32; }

inline EntityVersion GetEntityVersion(EntityId id) {
  uint64_t mask = std::numeric_limits<uint32_t>::max();
  return static_cast<EntityVersion>(id & mask);
}

inline bool IsEntityValid(EntityId id) { return (id >> 32) != EntityIndex(-1); }

struct Transform {
  vec3 position;
};
/*
class Scene {
 public:
  struct EntityDescriptor {
    EntityId id;
    ComponentMask mask;
  };
  Scene() {}

  ~Scene() {
    for (EntityDescriptor& desc : mEntities) {
      if (IsEntityValid(desc.id)) {
        this->DestroyEntity(desc.id);
      }
    }
    for (auto p : mPools) {
      delete p;
    }
  }

  EntityId CreateEntity() {
    if (!mFreeList.empty()) {
      EntityIndex index = mFreeList.back();
      mFreeList.pop_back();
      EntityId& freeId = mEntities[index].id;
      assert(!IsEntityValid(freeId));
      EntityVersion version = GetEntityVersion(freeId);
      EntityId id = CreateEntityId(index, version + 1);
      freeId = id;
      return id;
    }
    EntityIndex index = static_cast<EntityIndex>(mEntities.size());
    EntityId id = CreateEntityId(index, 0);
    mEntities.push_back({id, ComponentMask()});
    for (Pool* pool : mPools) {
      if (index >= pool->Size()) {
        pool->Resize(index + 1);
      }
    }
    return id;
  }

  template <typename T>
  T* AddComponent(EntityId id) {
    assert(IsEntityValid(id));
    ComponentId componentId = GetComponentId<T>();
    if (componentId >= mPools.size()) {
      mPools.resize(componentId + 1, nullptr);
    }
    EntityIndex index = GetEntityIndex(id);
    if (mPools[componentId] == nullptr) {
      Pool* pool = new ChunkedPool<T>(sizeof(T));
      pool->Resize(index + 1);
      mPools[componentId] = pool;
    }
    T* component = new (mPools[componentId]->Get(index)) T();
    mEntities[index].mask.set(componentId);
    return component;
  }

  template <typename T>
  T* GetComponent(EntityId id) {
    assert(IsEntityValid(id));
    EntityIndex index = GetEntityIndex(id);
    ComponentId componentId = GetComponentId<T>();
    if (!mEntities[index].mask.test(componentId)) {
      return nullptr;
    }
    return static_cast<T*>(mPools[componentId]->Get(index));
  }

  template <typename T>
  void RemoveComponent(EntityId id) {
    assert(IsEntityValid(id));
    EntityIndex index = GetEntityIndex(id);
    ComponentId componentId = GetComponentId<T>();
    ComponentMask& mask = mEntities[index].mask;
    if (mask.test(componentId)) {
      mPools[componentId]->Delete(index);
      mask.reset(componentId);
    }
  }

  void DestroyEntity(EntityId id) {
    assert(IsEntityValid(id));
    EntityIndex index = GetEntityIndex(id);
    ComponentMask& mask = mEntities[index].mask;
    for (ComponentId component = 0; component < mask.size(); ++component) {
      if (mask.test(component)) {
        mPools[component]->Delete(index);
      }
    }
    mask.reset();
    EntityId invalid = CreateEntityId(EntityIndex(-1), GetEntityVersion(id));
    mEntities[index].id = invalid;
    mFreeList.push_back(index);
  }

  vector<EntityIndex> mFreeList;
  vector<EntityDescriptor> mEntities;
  vector<Pool*> mPools;
};
*/

class Scene {
 public:
  Scene() {}
  virtual ~Scene() {}

  EntityId CreateEntity() {
    if (!mFreeList.empty()) {
      EntityIndex index = mFreeList.back();
      mFreeList.pop_back();
      EntityId& freeId = mEntities[index];
      assert(!IsEntityValid(freeId));
      EntityVersion version = GetEntityVersion(freeId);
      EntityId id = CreateEntityId(index, version + 1);
      freeId = id;
      return id;
    }
    EntityIndex index = static_cast<EntityIndex>(mEntities.size());
    EntityId id = CreateEntityId(index, 0);
    mEntities.push_back(id);
    return id;
  }

  template <typename T>
  T* AddComponent(EntityId id) {
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
  T* GetComponent(EntityId id) {
    EntityIndex index = GetEntityIndex(id);
    ComponentId componentId = GetComponentId<T>();
    assert(IsEntityValid(id));
    assert(componentId < mComponents.size());
    Pool* pool = mComponents[componentId];
    return static_cast<T*>(pool->Get(index));
  }

  template <typename T>
  void RemoveComponent(EntityId id) {
    EntityIndex index = GetEntityIndex(id);
    ComponentId componentId = GetComponentId<T>();
    assert(IsEntityValid(id));
    assert(componentId < mComponents.size());
    Pool* pool = mComponents[componentId];
    pool->Delete(index);
  }

  void DestroyEntity(EntityId id) {
    EntityIndex index = GetEntityIndex(id);
    EntityVersion version = GetEntityVersion(id);
    for (Pool* pool : mComponents) {
      if (pool->Get(index)) {
        pool->Delete(index);
      }
    }
    mEntities[index] = CreateEntityId(EntityIndex(-1), version);
    mFreeList.push_back(index);
  }

  vector<EntityIndex> mFreeList;
  vector<EntityId> mEntities;
  vector<Pool*> mComponents;
};

#endif