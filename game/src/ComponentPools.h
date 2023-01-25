#ifndef GAME_COMPONENT_HUB_H
#define GAME_COMPONENT_HUB_H

#include "EcsTypes.h"
#include "Pool.h"

class ComponentPools {
 public:
  protected:
  Pool* pools[MAX_COMPONENTS];
};

#endif