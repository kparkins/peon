#ifndef PEON_PHYSICS_SYSTEM_H
#define PEON_PHYSICS_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "RigidBody.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"
#include "entity/Scene.h"
#include "event/Bus.h"
#include "log/Logger.h"
#include "profile/BlockTimer.h"

using glm::mat4;
using glm::value_ptr;
using glm::vec3;
using std::vector;

using Peon::Bus;
using Peon::MakeShared;
using Peon::Shared;

namespace Peon {

class PhysicsSystem : public Peon::Uncopyable {
 public:
  PhysicsSystem(Shared<Scene> scene, Shared<Bus> bus);
  ~PhysicsSystem();

  void Update(double dt, int maxSteps = 1, float timeStep = 1.f / 60.f);
  void SyncTransform(Entity* entity);
  void AddRigidBody(Component<RigidBody> body);
  void RemoveRigidBody(Component<RigidBody> body);
  inline void SetGravity(vec3 gravity);

 protected:
  Shared<Bus> mBus;
  Shared<Scene> mScene;
  btDynamicsWorld* btWorld;
  btCollisionDispatcher* btDispatcher;
  btBroadphaseInterface* btBroadphase;
  btConstraintSolver* btSolver;
  btCollisionConfiguration* btConfiguration;
};

}  // namespace Peon

inline void Peon::PhysicsSystem::SetGravity(vec3 gravity) {
  btWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

#endif