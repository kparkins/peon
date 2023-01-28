#ifndef PEON_GL_RENDERER_H
#define PEON_GL_RENDERER_H

#include <glm/glm.hpp>

#include "GLViewport.h"
#include "common/TypeAliases.h"
#include "GraphicsExtensions.h"

using glm::vec4;

namespace Peon {
enum GLOption {
  DEPTH_TEST = GL_DEPTH_TEST,
  MSAA = GL_MULTISAMPLE,
  CULL_FACE = GL_CULL_FACE,
};

enum GLDepthFunc {
  LESS = GL_LESS,
  ALWAYS = GL_ALWAYS,
  NEVER = GL_NEVER,
  EQUAL = GL_EQUAL,
  LESS_EQUAL = GL_LEQUAL,
  GREATER = GL_GREATER,
  NOT_EQUAL = GL_NOTEQUAL,
  GREATER_EQUAL = GL_GEQUAL,
};

enum GLCullFace {
  FRONT = GL_FRONT,
  BACK = GL_BACK,
  FRONT_BACK = GL_FRONT_AND_BACK,
};

enum GLFrontFace {
  CCW = GL_CCW,
  CW = GL_CW,
};


class GLRenderer {
 public:
  GLRenderer();
  ~GLRenderer();

  void Enable(GLOption option);
  void Disable(GLOption option);

  void SetDepthFunc(GLDepthFunc func);
  void SetClearColor(const vec4& color);
  void SetViewport(const GLViewport& view);
  void SetCullFace(GLCullFace faces, GLFrontFace order);


  void Clear();

 protected:
  vec4 mClearColor;
  GLbitfield mClearBits;
};

}  // namespace Peon
#endif