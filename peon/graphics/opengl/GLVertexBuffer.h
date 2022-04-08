
#ifndef PEON_GL_VERTEX_BUFFER_H
#define PEON_GL_VERTEX_BUFFER_H

#include "GraphicsExtensions.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"

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

class GLVertexBuffer : private Uncopyable {
 public:
  GLVertexBuffer(GLuint vao, GLuint vbo, int size, int numELements)
      : mVao(vao), mVbo(vbo), mSize(size), mNumElements(numELements) {}

  template <typename... Attributes>
  static Shared<GLVertexBuffer> Create(int size, const void* data,
                                       Attributes&&... attributes) {
    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    int stride = GLVertexBuffer::CalculateStride(attributes...);
    GLVertexBuffer::MapAttribPointers(0, stride, 0, attributes...);

    glBindVertexArray(0);
    return MakeShared<GLVertexBuffer>(vao, vbo, size, size / stride);
  }

  void Bind();
  void Unbind();
  void Draw();

 protected:
  template <typename T>
  static inline int CalculateStride(T attribute) {
    return sizeof(attribute);
  }

  template <typename T, typename... Attributes>
  static inline int CalculateStride(T first, Attributes&&... attributes) {
    return sizeof(first) + CalculateStride(attributes...);
  }

  template <typename T>
  static inline void MapAttribPointers(int index, int stride, uint64_t offset,
                                       T first) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, T::NumElements, T::GLtype, GL_FALSE, stride,
                          reinterpret_cast<GLvoid*>(offset));
  }

  template <typename T, typename... Attributes>
  static inline void MapAttribPointers(int index, int stride, uint64_t offset,
                                       T first, Attributes&&... attributes) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, T::NumElements, T::GLtype, GL_FALSE, stride,
                          reinterpret_cast<GLvoid*>(offset));
    MapAttribPointers(index + 1, stride, offset + sizeof(T), attributes...);
  }

  GLuint mVao;
  GLuint mVbo;
  int mSize;
  int mNumElements;
};

}  // namespace Peon

#endif