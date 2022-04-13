#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <cstdint>

typedef uint64_t ComponentId;
const uint32_t MAX_COMPONENTS = 32;
extern uint64_t gLastComponentId;

template <class T>
ComponentId GetComponentId() {
  static ComponentId componentId = gLastComponentId++;
  return componentId;
}

#endif