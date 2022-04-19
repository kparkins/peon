#ifndef GAME_COLLISION_SHAPE_H
#define GAME_COLLISION_SHAPE_H

#include <bullet/btBulletCollisionCommon.h>

#include <glm/glm.hpp>

#include "common/TypeAliases.h"

using glm::vec3;
using Peon::MakeShared;
using Peon::Shared;

struct CollisionShape {
  CollisionShape(btCollisionShape* shape) : shape(shape) {}
  static CollisionShape Box(vec3 halfExtents) {
    return CollisionShape(
        new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z)));
  }
  static CollisionShape Sphere(float radius) {
    return CollisionShape(new btSphereShape(radius));
  }
  Shared<btCollisionShape> shape;
};

#endif