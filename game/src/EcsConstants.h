#ifndef GAME_ECS_CONSTANTS_H
#define GAME_ECS_CONSTANTS_H

#include <bitset>

using std::bitset;

typedef uint64_t ComponentId;
const uint32_t MAX_COMPONENTS = 64;
typedef bitset<MAX_COMPONENTS> ComponentMask;

typedef uint64_t EntityId;
typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
const EntityIndex INVALID_ENTITY_INDEX = EntityIndex(-1);

#endif