#include "Physics.h"

PhysicsEngine::PhysicsEngine() {
  btConfiguration = new btDefaultCollisionConfiguration();
  btDispatcher = new btCollisionDispatcher(btConfiguration);
  btBroadphase = new btDbvtBroadphase();
  btSolver = new btSequentialImpulseConstraintSolver();
  btWorld = new btDiscreteDynamicsWorld(btDispatcher, btBroadphase, btSolver,
                                        btConfiguration);
  btWorld->setGravity(btVector3(0, -10, 0));
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(btVector3(0, 0, 0));
  btStaticPlaneShape* plane =
      new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
  btMotionState* motion = new btDefaultMotionState(btTransform);
  btRigidBody::btRigidBodyConstructionInfo info(btScalar(0), motion, plane);
  ground = new btRigidBody(info);
  ground->setRollingFriction(.5f);
  ground->setRestitution(1.f);
  btWorld->addRigidBody(ground);
}

PhysicsEngine::~PhysicsEngine() {
  delete btWorld;
  delete btSolver;
  delete btBroadphase;
  delete btDispatcher;
  delete btConfiguration;
  for (btRigidBody* body : bodies) {
    btCollisionShape* shape = body->getCollisionShape();
    btMotionState* motionState = body->getMotionState();
    delete body;
    if (motionState) {
      delete motionState;
    }
    if (shape) {
      delete shape;
    }
  }
  delete ground;
}

vector<btRigidBody*> PhysicsEngine::GetRigidBodies() const {
  return this->bodies;
}

void PhysicsEngine::StepSimulation(float dt) { btWorld->stepSimulation(dt); }

btRigidBody* PhysicsEngine::AddSphere(float radius, vec3 position, float mass) {
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(btVector3(position.x, position.y, position.z));
  btSphereShape* sphereShape = new btSphereShape(radius);
  btVector3 inertia(0, 0, 0);
  if (mass != 0) {
    sphereShape->calculateLocalInertia(mass, inertia);
  }
  btMotionState* motion = new btDefaultMotionState(btTransform);
  btRigidBody::btRigidBodyConstructionInfo info(btScalar(mass), motion,
                                                sphereShape, inertia);
  btRigidBody* body = new btRigidBody(info);
  body->setRollingFriction(.5f);
  btWorld->addRigidBody(body);
  bodies.push_back(body);
  return body;
}