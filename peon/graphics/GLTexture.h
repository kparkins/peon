#ifndef PEON_GL_TEXTURE_H
#define PEON_GL_TEXTURE_H

#include "GraphicsExtensions.h"

namespace Peon {

enum GLTextureWrap {
  PEON_REPEAT = GL_REPEAT,
  PEON_MIRRORED = GL_MIRRORED_REPEAT,
  PEON_CLAMP_EDGE = GL_CLAMP_TO_EDGE,
  PEON_CLAMP_BORDER = GL_CLAMP_TO_BORDER
};

enum GLTextureFilter {
  PEON_NEAREST = GL_NEAREST,
  PEON_LINEAR = GL_LINEAR,
};

typedef struct GLTextureOpts {
} GLTextureOpts;

class GLTexture {
 public:
  static Shared<GLTexture> Load(const std::string& file) { return nullptr; }

 private:
  GLuint mId;
};
}  // namespace Peon

#endif