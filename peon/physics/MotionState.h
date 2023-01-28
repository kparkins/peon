#ifndef PEON_MOTION_STATE_H
#define PEON_MOTION_STATE_H

#include <bullet/btBulletDynamicsCommon.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "entity/Scene.h"

using glm::mat4;
using glm::value_ptr;

namespace Peon {

    ATTRIBUTE_ALIGNED16(struct) MotionState : public btDefaultMotionState{
     public:
      btTransform m_graphicsWorldTrans;
      btTransform m_centerOfMassOffset;
      btTransform m_startWorldTrans;
      void* m_userPointer;

      BT_DECLARE_ALIGNED_ALLOCATOR();

      MotionState(
          const btTransform& startTrans = btTransform::getIdentity(),
          const btTransform& centerOfMassOffset = btTransform::getIdentity());

      /// synchronizes world transform from user to physics
      virtual void getWorldTransform(btTransform& centerOfMassWorldTrans)
          const override;

      /// synchronizes world transform from physics to user
      /// Bullet only calls the update of worldtransform for active objects
      virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
          override;
    };

}

#endif