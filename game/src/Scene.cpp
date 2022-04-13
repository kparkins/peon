#include "Scene.h"

EntityId Scene::CreateEntity() {
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

void Scene::DestroyEntity(EntityId id) {
  EntityIndex index = GetEntityIndex(id);
  EntityVersion version = GetEntityVersion(id);
  for (Pool* pool : mComponents) {
    if (pool->Get(index)) {
      pool->Delete(index);
    }
  }
  mEntities[index] = CreateEntityId(INVALID_ENTITY_INDEX, version);
  mFreeList.push_back(index);
}