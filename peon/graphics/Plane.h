
#ifndef PEON_PLANE_H
#define PEON_PLANE_H

#include <iostream>
#include <vector>

#include "common/TypeAliases.h"
#include "opengl/GLVertexArray.h"
#include "opengl/GLVertexBuffer.h"

namespace Peon {

class Plane {
 public:
  static Shared<GLVertexArray> MakePlane(int numSizeTiles, float tileSize);

 private:
  static std::vector<float> build(int numSizeTiles, float tileSize);
};

}  // namespace Peon

#endif
