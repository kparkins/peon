#include "GLRenderer.h"

Peon::GLRenderer::GLRenderer()
    : mClearColor(vec4(0.f, 0.f, 0.f, 0.f)), mClearBits(GL_COLOR_BUFFER_BIT) {}

Peon::GLRenderer::~GLRenderer() {}

void Peon::GLRenderer::Enable(GLOption option) {
  glEnable(option);
  if (option == GLOption::DEPTH_TEST) {
    mClearBits |= GL_DEPTH_BUFFER_BIT;
  }
}

void Peon::GLRenderer::Disable(GLOption option) {
  glDisable(option);
  if (option == GLOption::DEPTH_TEST && (mClearBits & GL_DEPTH_BUFFER_BIT)) {
    mClearBits ^= GL_DEPTH_BUFFER_BIT;
  }
}

void Peon::GLRenderer::SetDepthFunc(GLDepthFunc func) { glDepthFunc(func); }

void Peon::GLRenderer::SetClearColor(const vec4& color) { mClearColor = color; }

void Peon::GLRenderer::SetViewport(const GLViewport& view) {
  glViewport(view.x, view.y, static_cast<GLsizei>(view.w),
             static_cast<GLsizei>(view.h));
}

void Peon::GLRenderer::SetCullFace(GLCullFace faces, GLFrontFace order) {
  glCullFace(faces);
  glFrontFace(order);
}

void Peon::GLRenderer::Clear() {
  glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
  glClear(mClearBits);
}
