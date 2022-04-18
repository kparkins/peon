#include "Physics.h"

void MakeGround() {
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(btVector3(0, 0, 0));
  btStaticPlaneShape* plane =
      new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
  btMotionState* motion = new btDefaultMotionState(btTransform);
  btRigidBody::btRigidBodyConstructionInfo info(btScalar(0), motion, plane);
  btRigidBody* ground = new btRigidBody(info);
  ground->setRollingFriction(.5f);
  ground->setRestitution(1.f);
}