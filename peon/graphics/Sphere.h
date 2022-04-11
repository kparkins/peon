#ifndef PEON_SPHERE_H
#define PEON_SPHERE_H

#include <vector>

#include "opengl/GLVertexArray.h"
#include "opengl/GLVertexBuffer.h"

namespace Peon {

class Sphere {
 public:
  static Shared<GLVertexArray> MakeSphere(float radius, int sectors = 36,
                                          int stacks = 18);

 private:
  static std::vector<float> build(float radius, int sectors, int stacks);
};

}  // namespace Peon

#endif
