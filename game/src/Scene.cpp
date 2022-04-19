#include "Scene.h"

Scene::~Scene() {
  for (Entity* entity : mEntities) {
    delete entity;
  }
}

Entity* Scene::CreateEntity() {
  if (!mFreeList.empty()) {
    EntityIndex index = mFreeList.back();
    mFreeList.pop_back();
    Entity* free = mEntities[index];
    assert(!free->IsValid());
    EntityVersion version = free->GetVersion();
    free->SetId(index, version + 1);
    free->mScene = this;
    free->mComponents.reset();
    this->AddComponent<Transform>(free);
    return free;
  }
  EntityIndex index = static_cast<EntityIndex>(mEntities.size());
  Entity* entity = new Entity(index, 0);
  this->AddComponent<Transform>(entity);
  entity->mScene = this;
  mEntities.push_back(entity);
  return entity;
}

void Scene::DestroyEntity(Entity* entity) {
  EntityIndex index = entity->GetIndex();
  EntityVersion version = entity->GetVersion();
  ComponentMask mask = entity->GetComponents();
  for (ComponentId id = 0; id < MAX_COMPONENTS; id++) {
    if (mask.test(id)) {
      Pool* pool = mComponents[id];
      pool->Destroy(index);
      pool->Free(index);
    }
  }
  entity->SetId(INVALID_ENTITY_INDEX, version);
  entity->mScene = nullptr;
  entity->mComponents.reset();
  mFreeList.push_back(index);
}

template <>
void Scene::RemoveComponent<Transform>(Entity* entity) {
  assert(false);
}