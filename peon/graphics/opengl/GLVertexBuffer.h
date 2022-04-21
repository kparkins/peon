#ifndef PEON_GL_VERTEX_BUFFER_H
#define PEON_GL_VERTEX_BUFFER_H

#include "GraphicsExtensions.h"
#include "common/TypeAliases.h"

namespace Peon {

template <int N, typename Type, unsigned int GL_TYPE>
struct GLVertexAttribute {
  static const int GLtype = GL_TYPE;
  static const int NumElements = N;
  Type type[N];
};

const auto GLAttribute4f = GLVertexAttribute<4, GLfloat, GL_FLOAT>();
const auto GLAttribute3f = GLVertexAttribute<3, GLfloat, GL_FLOAT>();
const auto GLAttribute2f = GLVertexAttribute<2, GLfloat, GL_FLOAT>();
const auto GLAttribute4d = GLVertexAttribute<4, GLdouble, GL_DOUBLE>();
const auto GLAttribute3d = GLVertexAttribute<3, GLdouble, GL_DOUBLE>();
const auto GLAttribute2d = GLVertexAttribute<2, GLdouble, GL_DOUBLE>();

}  // namespace Peon
#endif