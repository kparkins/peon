#ifndef GAME_COLLISION_SHAPE_H
#define GAME_COLLISION_SHAPE_H

#include <bullet/btBulletCollisionCommon.h>

#include <glm/glm.hpp>

#include "common/TypeAliases.h"

using glm::vec3;
using Peon::MakeUnique;
using Peon::Unique;

typedef btCollisionShape CollisionShape;

class CollisionShapes {
 public:
  static Unique<CollisionShape> NewBox(vec3 halfExtents) {
    return MakeUnique<btBoxShape>(
        btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
  }
  static Unique<CollisionShape> NewSphere(float radius) {
    return MakeUnique<btSphereShape>(radius);
  }
};

#endif