#ifndef PEON_MODEL_H
#define PEON_MODEL_H

#include "opengl/GLVertexArray.h"

namespace Peon {

typedef struct Model {
  Model(Shared<GLVertexArray> buffer) : buffer(buffer) {}
  Shared<GLVertexArray> buffer;
} Model;

}  // namespace Peon

#endif