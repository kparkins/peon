#ifndef GAME_ECS_TYPES_H
#define GAME_ECS_TYPES_H

#include <bitset>
#include <cstdint>
#include <glm/glm.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec4;
using std::bitset;
using std::numeric_limits;

typedef uint32_t ComponentId;
const uint32_t MAX_COMPONENTS = 64;
typedef bitset<MAX_COMPONENTS> ComponentMask;

typedef uint64_t EntityId;
typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
const EntityIndex INVALID_ENTITY_INDEX = EntityIndex(-1);
const EntityId INVALID_ENTITY_ID = EntityId(-2);

inline EntityVersion GetEntityVersion(EntityId id) {
  return static_cast<EntityVersion>(id & numeric_limits<uint32_t>::max());
}

inline EntityIndex GetEntityIndex(EntityId id) { return id >> 32; }

inline bool IsEntityIdValid(EntityId id) {
  return (id >> 32) != INVALID_ENTITY_INDEX;
}

typedef struct EntitySlot {
  EntityId id;
  ComponentMask components;
} EntitySlot;

typedef struct Transform {
  vec3 GetWorldPosition() const {
    return vec3(matrix * vec4(0.f, 0.f, 0.f, 1.f));
  }
  mat4 matrix;
} Transform;

#endif