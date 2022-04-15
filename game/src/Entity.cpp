#include "Entity.h"

Entity::Entity() : mId(INVALID_ENTITY_INDEX) {}

Entity::Entity(EntityIndex index, EntityVersion version)
    : mId(INVALID_ENTITY_INDEX) {
  this->SetId(index, version);
}

Entity::~Entity() {}

void Entity::SetId(EntityIndex index, EntityVersion version) {
  this->mId = static_cast<EntityId>(index) << 32;
  this->mId |= static_cast<EntityId>(version);
}