#ifndef PEON_RENDERABLE_H
#define PEON_RENDERABLE_H

#include "entity/Scene.h"
#include "opengl/GLRenderer.h"

namespace Peon {
class Renderable {
 public:
  virtual void Render(GLRenderer* renderer, Scene* scene, Camera* camera) = 0;
};

}  // namespace Peon
#endif  // !GAME_RENDERABLE_H
