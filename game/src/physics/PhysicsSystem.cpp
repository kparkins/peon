#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(Shared<Scene> scene, Shared<Bus> bus)
    : mBus(bus), mScene(scene) {
  btConfiguration = new btDefaultCollisionConfiguration();
  btDispatcher = new btCollisionDispatcher(btConfiguration);
  btBroadphase = new btDbvtBroadphase();
  btSolver = new btSequentialImpulseConstraintSolver();
  btWorld = new btDiscreteDynamicsWorld(btDispatcher, btBroadphase, btSolver,
                                        btConfiguration);
  btWorld->setGravity(btVector3(0, -10, 0));
}

PhysicsSystem::~PhysicsSystem() {
  delete btWorld;
  delete btSolver;
  delete btBroadphase;
  delete btDispatcher;
  delete btConfiguration;
}

void PhysicsSystem::Update(double dt, int maxSteps, float timeStep) {
  btWorld->stepSimulation(btScalar(dt), maxSteps, timeStep);
}

void PhysicsSystem::AddRigidBody(Component<RigidBody> body) {
  assert(body);
  assert(body->mWorld == nullptr);
  body->mWorld = this->btWorld;
  btWorld->addRigidBody(body->mBody.get());
}

void PhysicsSystem::SyncTransform(Component<RigidBody> body) {
  btTransform transform;
  Entity* entity = body.GetEntity();
  auto component = entity->GetComponent<Transform>();

  transform.setFromOpenGLMatrix(value_ptr(component->matrix));
  body->mBody->setWorldTransform(transform);

  if (body->mMotionState) {
    body->mMotionState->m_userPointer = body.GetEntity();
    body->mBody->getMotionState()->setWorldTransform(transform);
  }
}

void PhysicsSystem::RemoveRigidBody(Component<RigidBody> body) {
  assert(body->mWorld == this->btWorld);
  body->mWorld = nullptr;
  body->mMotionState->m_userPointer = nullptr;
  btWorld->removeRigidBody(body->mBody.get());
}