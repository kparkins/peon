#ifndef GAME_RIGID_BODY_H
#define GAME_RIGID_BODY_H

#include <bullet/btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

#include "CollisionShape.h"
#include "MotionState.h"
#include "common/TypeAliases.h"

using glm::mat4;
using glm::vec3;

using Peon::MakeShared;
using Peon::MakeUnique;
using Peon::Shared;
using Peon::Unique;

class RigidBody {
 public:
  RigidBody(float mass, Unique<CollisionShape> shape);

  ~RigidBody();
  RigidBody(const RigidBody& body) = delete;
  RigidBody(RigidBody&& body);

  RigidBody& operator=(const RigidBody& body) = delete;
  RigidBody& operator=(RigidBody&& body);

  void SetCollisionShape(Unique<CollisionShape> shape);

  inline void SetMass(float mass);
  inline void SetInertia(vec3 inertia);
  inline void SetFriction(float f);
  inline void SetRollingFriction(float f);
  inline void SetSpinningFriction(float f);
  inline void SetRestitution(float f);
  inline void ApplyCentralImpulse(vec3 impulse);

  inline float GetFriction() const;
  inline float GetRollingFriction() const;
  inline float GetRestitution() const;
  inline float GetMass() const;

  inline void UpdateBodyInertia();
  inline mat4 GetWorldTransform();

 private:
  friend class PhysicsSystem;
  float mMass;
  btVector3 mInertia;
  btDynamicsWorld* mWorld;
  Unique<btCollisionShape> mShape;
  Unique<btRigidBody> mBody;
  Unique<MotionState> mMotionState;
};

inline void RigidBody::SetMass(float mass) {
  assert(mass >= 0);
  this->mMass = mass;
  this->UpdateBodyInertia();
}

inline void RigidBody::SetInertia(vec3 inertia) {
  this->mInertia = btVector3(inertia.x, inertia.y, inertia.z);
  this->UpdateBodyInertia();
}

inline void RigidBody::SetFriction(float f) { mBody->setFriction(f); }

inline void RigidBody::SetRollingFriction(float f) {
  mBody->setRollingFriction(f);
}

inline void RigidBody::SetSpinningFriction(float f) {
  mBody->setSpinningFriction(f);
}

inline void RigidBody::SetRestitution(float f) { mBody->setRestitution(f); }

inline void RigidBody::ApplyCentralImpulse(vec3 impulse) {
  btVector3 i(impulse.x, impulse.y, impulse.z);
  mBody->applyCentralImpulse(i);
  this->UpdateBodyInertia();
}

inline float RigidBody::GetFriction() const { return mBody->getFriction(); }

inline float RigidBody::GetRollingFriction() const {
  return mBody->getFriction();
}
inline float RigidBody::GetRestitution() const {
  return mBody->getRestitution();
}

inline float RigidBody::GetMass() const { return this->mMass; }

inline void RigidBody::UpdateBodyInertia() {
  if (this->mMass != 0) {
    mShape->calculateLocalInertia(this->mMass, this->mInertia);
    mBody->setMassProps(this->mMass, this->mInertia);
    mBody->updateInertiaTensor();
  }
}

inline mat4 RigidBody::GetWorldTransform() {
  mat4 matrix(0.f);
  btTransform transform = mBody->getWorldTransform();
  transform.getOpenGLMatrix(value_ptr(matrix));
  return matrix;
}

#endif