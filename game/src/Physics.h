#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Scene.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "common/TypeAliases.h"
#include "event/Bus.h"
#include "log/Logger.h"

using glm::mat4;
using glm::value_ptr;
using glm::vec3;
using std::vector;

namespace Peon {

class PhysicsSystem;

struct CollisionShape {
  CollisionShape() : shape(MakeShared<btSphereShape>(1.f)) {}
  CollisionShape(btCollisionShape* shape) : shape(shape) {}
  Shared<btCollisionShape> shape;
};

ATTRIBUTE_ALIGNED16(struct) MotionState : public btDefaultMotionState {
 public:
  btTransform m_graphicsWorldTrans;
  btTransform m_centerOfMassOffset;
  btTransform m_startWorldTrans;
  void* m_userPointer;

  BT_DECLARE_ALIGNED_ALLOCATOR();

  MotionState(
      const btTransform& startTrans = btTransform::getIdentity(),
      const btTransform& centerOfMassOffset = btTransform::getIdentity())
      : m_graphicsWorldTrans(startTrans),
        m_centerOfMassOffset(centerOfMassOffset),
        m_startWorldTrans(startTrans),
        m_userPointer(0)

  {}

  /// synchronizes world transform from user to physics
  virtual void getWorldTransform(btTransform & centerOfMassWorldTrans)
      const override {
    centerOfMassWorldTrans =
        m_graphicsWorldTrans * m_centerOfMassOffset.inverse();
  }

  /// synchronizes world transform from physics to user
  /// Bullet only calls the update of worldtransform for active objects
  virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
      override {
    m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset;
    if (m_userPointer) {
      mat4* m = static_cast<mat4*>(m_userPointer);
      m_graphicsWorldTrans.getOpenGLMatrix(value_ptr(*m));
    }
  }

 protected:
  function<void(const mat4& transform)> mCallback;
  Shared<PhysicsSystem> system;
};

class RigidBody {
 public:
  RigidBody() : RigidBody(0.f, vec3(0, 0, 0), CollisionShape()) {}
  RigidBody(float mass, vec3 position, CollisionShape shape)
      : mMass(mass), mShape(move(shape.shape)) {
    mInertia = btVector3(0, 0, 0);
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(btVector3(position.x, position.y, position.z));
    btVector3 inertia(0, 0, 0);
    if (mass != 0) {
      mShape->calculateLocalInertia(mass, inertia);
    }
    this->mMotionState = MakeShared<MotionState>(btTransform);
    btRigidBody::btRigidBodyConstructionInfo info(
        btScalar(mass), mMotionState.get(), mShape.get(), inertia);
    this->mBody = MakeShared<btRigidBody>(info);
  }

  ~RigidBody() { LOG_INFO("Destroying rigid body "); }

  void SetCollionShape(CollisionShape* shape) {
    assert(shape);
    if (mWorld) {
      mWorld->removeRigidBody(mBody.get());
    }
    mShape.reset();
    mShape = shape->shape;
    mBody->setCollisionShape(mShape.get());
    this->UpdateBodyInertia();
    if (mWorld) {
      mWorld->addRigidBody(mBody.get());
    }
  }

  void SetMass(float mass) {
    assert(mass >= 0);
    this->mMass = mass;
    this->UpdateBodyInertia();
  }

  void SetInertia(vec3 inertia) {
    this->mInertia = btVector3(inertia.x, inertia.y, inertia.z);
    this->UpdateBodyInertia();
  }

  void SetFriction(float f) { mBody->setFriction(f); }
  void SetRollingFriction(float f) { mBody->setRollingFriction(f); }
  void SetRestitution(float f) { mBody->setRestitution(f); }
  void ApplyCentralImpulse(vec3 impulse) {
    btVector3 i(impulse.x, impulse.y, impulse.z);
    mBody->applyCentralImpulse(i);
  }

  float GetFriction() const { return mBody->getFriction(); }
  float GetRollingFriction() const { return mBody->getFriction(); }
  float GetRestitution() const { return mBody->getRestitution(); }
  float GetMass() const { return this->mMass; }

  inline void UpdateBodyInertia() {
    if (this->mMass != 0) {
      mShape->calculateLocalInertia(this->mMass, this->mInertia);
      mBody->setMassProps(this->mMass, this->mInertia);
      mBody->updateInertiaTensor();
    }
  }

  inline mat4 GetWorldTransform() {
    mat4 matrix(0.f);
    btTransform transform = mBody->getWorldTransform();
    transform.getOpenGLMatrix(value_ptr(matrix));
    return matrix;
  }

  friend class PhysicsSystem;
  float mMass;
  mat4 mWorldTransform;
  btVector3 mInertia;
  btDynamicsWorld* mWorld;
  Shared<btCollisionShape> mShape;
  Shared<btRigidBody> mBody;
  Shared<MotionState> mMotionState;
};

class PhysicsSystem : public Uncopyable {
 public:
  PhysicsSystem(Shared<Scene> scene, Shared<Bus> bus)
      : mBus(bus), mScene(scene) {
    btConfiguration = new btDefaultCollisionConfiguration();
    btDispatcher = new btCollisionDispatcher(btConfiguration);
    btBroadphase = new btDbvtBroadphase();
    btSolver = new btSequentialImpulseConstraintSolver();
    btWorld = new btDiscreteDynamicsWorld(btDispatcher, btBroadphase, btSolver,
                                          btConfiguration);
    btWorld->setGravity(btVector3(0, -10, 0));
  }

  ~PhysicsSystem() {
    delete btWorld;
    delete btSolver;
    delete btBroadphase;
    delete btDispatcher;
    delete btConfiguration;
  }

  void Update(float dt) { btWorld->stepSimulation(dt); }

  void AddRigidBody(RigidBody* body) {
    assert(body);
    assert(body->mWorld == nullptr);
    body->mWorld = this->btWorld;
    btWorld->addRigidBody(body->mBody.get());
  }

  void AddRigidBody(Component<RigidBody> body) {
    assert(body);
    assert(body->mWorld == nullptr);
    body->mWorld = this->btWorld;
    btWorld->addRigidBody(body->mBody.get());
  }

  void RemoveRigidBody(RigidBody* body) {
    assert(body->mWorld == this->btWorld);
    body->mWorld = nullptr;
    btWorld->removeRigidBody(body->mBody.get());
  }

  void RemoveRigidBody(Component<RigidBody> body) {
    assert(body->mWorld == this->btWorld);
    body->mWorld = nullptr;
    btWorld->removeRigidBody(body->mBody.get());
  }

  void SetGravity(vec3 gravity) {
    btWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
  }

 protected:
  Shared<Bus> mBus;
  Shared<Scene> mScene;
  vector<Shared<RigidBody>> mBodies;
  btDynamicsWorld* btWorld;
  btCollisionDispatcher* btDispatcher;
  btBroadphaseInterface* btBroadphase;
  btConstraintSolver* btSolver;
  btCollisionConfiguration* btConfiguration;
};

}  // namespace Peon
#endif