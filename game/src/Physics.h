#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <vector>

#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

using glm::vec3;
using std::vector;

class PhysicsEngine {
 public:
  PhysicsEngine();
  ~PhysicsEngine();

  void StepSimulation(float dt);
  btRigidBody* AddSphere(float radius, vec3 position, float mass);
  vector<btRigidBody*> GetRigidBodies() const;

 protected:
  btDynamicsWorld* btWorld;
  btCollisionDispatcher* btDispatcher;
  btBroadphaseInterface* btBroadphase;
  btConstraintSolver* btSolver;
  btCollisionConfiguration* btConfiguration;
  vector<btRigidBody*> bodies;
  btRigidBody* ground;
};

#endif