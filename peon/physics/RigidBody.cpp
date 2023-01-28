#include "RigidBody.h"

Peon::RigidBody::RigidBody(float mass, Unique<CollisionShape> shape)
    : mMass(mass), mShape(move(shape)) {
  mInertia = btVector3(0, 0, 0);
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(btVector3(0.f, 0.f, 0.f));
  btVector3 inertia(0, 0, 0);
  if (mass != 0) {
    mShape->calculateLocalInertia(mass, inertia);
    this->mMotionState = MakeUnique<MotionState>(btTransform);
  }
  btRigidBody::btRigidBodyConstructionInfo info(
      btScalar(mass), mMotionState.get(), mShape.get(), inertia);
  this->mBody = MakeUnique<btRigidBody>(info);
}

Peon::RigidBody::RigidBody(RigidBody&& body) {
  this->mMass = body.mMass;
  this->mInertia = body.mInertia;
  this->mWorld = body.mWorld;
  mShape = move(body.mShape);
  mBody = move(body.mBody);
  mMotionState = move(body.mMotionState);
}

Peon::RigidBody& Peon::RigidBody::operator=(RigidBody&& body) {
  this->mMass = body.mMass;
  this->mInertia = body.mInertia;
  this->mWorld = body.mWorld;
  mShape = move(body.mShape);
  mBody = move(body.mBody);
  mMotionState = move(body.mMotionState);
  return *this;
}

Peon::RigidBody::~RigidBody() {
  if (mWorld) {
    mWorld->removeRigidBody(mBody.get());
    mWorld = nullptr;
  }
  if (mMotionState) {
    this->mMotionState->m_userPointer = nullptr;
  }
}

void Peon::RigidBody::SetCollisionShape(Unique<CollisionShape> shape) {
  if (mWorld) {
    mWorld->removeRigidBody(mBody.get());
  }
  mShape.reset();
  mShape = move(shape);
  mBody->setCollisionShape(mShape.get());
  if (mWorld) {
    mWorld->addRigidBody(mBody.get());
  }
}
