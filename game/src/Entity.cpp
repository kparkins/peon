#include "Entity.h"

Entity::Entity() : mId(INVALID_ENTITY_INDEX) {}

Entity::Entity(EntityIndex index, EntityVersion version)
    : mId(CreateEntityId(index, version)) {}

Entity::~Entity() {}