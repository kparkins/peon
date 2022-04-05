#ifndef PEON_GL_TEXTURE_H
#define PEON_GL_TEXTURE_H

#include <stb/stb_image.h>

#include <string>

#include "GraphicsExtensions.h"
#include "common/TypeAliases.h"

using std::string;

namespace Peon {

enum GLTextureUnit {
  TEXTURE0 = GL_TEXTURE0,
  TEXTURE1 = GL_TEXTURE1,
  TEXTURE2 = GL_TEXTURE2,
  TEXTURE3 = GL_TEXTURE3,
  TEXTURE4 = GL_TEXTURE4,
  TEXTURE5 = GL_TEXTURE5,
  TEXTURE6 = GL_TEXTURE6,
  TEXTURE7 = GL_TEXTURE7,
  TEXTURE8 = GL_TEXTURE8,
  TEXTURE9 = GL_TEXTURE9,
  TEXTURE10 = GL_TEXTURE10,
  TEXTURE11 = GL_TEXTURE11,
  TEXTURE12 = GL_TEXTURE12,
  TEXTURE13 = GL_TEXTURE13,
  TEXTURE14 = GL_TEXTURE14,
  TEXTURE15 = GL_TEXTURE15,
};

enum GLTextureWrap {
  REPEAT = GL_REPEAT,
  MIRRORED = GL_MIRRORED_REPEAT,
  CLAMP_EDGE = GL_CLAMP_TO_EDGE,
  CLAMP_BORDER = GL_CLAMP_TO_BORDER
};

enum GLTextureFilter {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

enum GLTextureTarget {
  TEXTURE_1D = GL_TEXTURE_1D,
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_3D = GL_TEXTURE_3D,
  TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
  TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
  TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
  TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
  TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
  TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
  TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
};

typedef struct GLTextureOpts {
  GLTextureTarget Target = TEXTURE_2D;
  GLTextureUnit TextureUnit = TEXTURE0;
  GLTextureWrap S = GLTextureWrap::REPEAT;
  GLTextureWrap T = GLTextureWrap::REPEAT;
  GLTextureWrap R = GLTextureWrap::REPEAT;
  GLTextureFilter MinFilter = GLTextureFilter::LINEAR_MIPMAP_LINEAR;
  GLTextureFilter MaxFilter = GLTextureFilter::LINEAR;
  bool GenerateMipMaps = true;
} GLTextureOpts;

class GLTexture {
 public:
  explicit GLTexture(const string& file, GLTextureOpts opts = GLTextureOpts());
  ~GLTexture();

  void Load(const string& file);
  void Bind();
  void Unbind();
  void SetTextureUnit(GLTextureUnit unit);

  static Shared<GLTexture> FromFile(const string& file,
                                    GLTextureOpts opts = GLTextureOpts());

 private:
  GLuint mTexture;
  GLTextureOpts mOpts;
};
}  // namespace Peon

#endif