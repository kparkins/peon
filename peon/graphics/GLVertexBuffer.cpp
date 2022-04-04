#include "GLVertexBuffer.h"

void Peon::GLVertexBuffer::Bind() { glBindVertexArray(mVao); }

void Peon::GLVertexBuffer::Unbind() { glBindVertexArray(0); }

void Peon::GLVertexBuffer::Draw() {
  glDrawArrays(GL_TRIANGLES, 0, mNumElements);
}