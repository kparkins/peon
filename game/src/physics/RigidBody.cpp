#include "RigidBody.h"

RigidBody::RigidBody(float mass, CollisionShape shape)
    : mMass(mass), mShape(move(shape.shape)) {
  mInertia = btVector3(0, 0, 0);
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(btVector3(0.f, 0.f, 0.f));
  btVector3 inertia(0, 0, 0);
  if (mass != 0) {
    mShape->calculateLocalInertia(mass, inertia);
  }
  this->mMotionState = MakeShared<MotionState>(btTransform);
  btRigidBody::btRigidBodyConstructionInfo info(
      btScalar(mass), mMotionState.get(), mShape.get(), inertia);
  this->mBody = MakeShared<btRigidBody>(info);
}

RigidBody::~RigidBody() { this->mMotionState->m_userPointer = nullptr; }

void RigidBody::SetCollisionShape(const CollisionShape& shape) {
  if (mWorld) {
    mWorld->removeRigidBody(mBody.get());
  }
  mShape.reset();
  mShape = shape.shape;
  mBody->setCollisionShape(mShape.get());
  this->UpdateBodyInertia();
  if (mWorld) {
    mWorld->addRigidBody(mBody.get());
  }
}
