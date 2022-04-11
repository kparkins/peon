#include "GLVertexArray.h"

void Peon::GLVertexArray::Bind() { glBindVertexArray(mVao); }

void Peon::GLVertexArray::Unbind() { glBindVertexArray(0); }

void Peon::GLVertexArray::Draw() {
  glDrawArrays(GL_TRIANGLES, 0, mNumElements);
}