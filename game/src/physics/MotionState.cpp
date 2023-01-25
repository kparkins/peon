#include "MotionState.h"

MotionState::MotionState(const btTransform& startTrans,
                         const btTransform& centerOfMassOffset)
    : m_graphicsWorldTrans(startTrans),
      m_centerOfMassOffset(centerOfMassOffset),
      m_startWorldTrans(startTrans),
      m_userPointer(0) {}

void MotionState::getWorldTransform(btTransform& centerOfMassWorldTrans) const {
  centerOfMassWorldTrans =
      m_graphicsWorldTrans * m_centerOfMassOffset.inverse();
}

void MotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans) {
  m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset;
  if (m_userPointer) {
    Entity* entity = static_cast<Entity*>(m_userPointer);
    Component<Transform> transform = entity->GetComponent<Transform>();
    m_graphicsWorldTrans.getOpenGLMatrix(value_ptr(transform->matrix));
  }
}