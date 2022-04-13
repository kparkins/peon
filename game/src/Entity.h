#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <cstdint>

#include "Pool.h"

typedef uint64_t EntityId;
typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
const EntityIndex INVALID_ENTITY_INDEX = EntityIndex(-1);

inline EntityId CreateEntityId(EntityIndex index, EntityVersion version) {
  return (static_cast<EntityId>(index) << 32) | static_cast<EntityId>(version);
}

inline EntityIndex GetEntityIndex(EntityId id) { return id >> 32; }

inline EntityVersion GetEntityVersion(EntityId id) {
  uint64_t mask = std::numeric_limits<uint32_t>::max();
  return static_cast<EntityVersion>(id & mask);
}

inline bool IsEntityValid(EntityId id) {
  return (id >> 32) != INVALID_ENTITY_INDEX;
}

#endif