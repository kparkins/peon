#include "GLTexture2D.h"

Peon::GLTexture2D::GLTexture2D(const std::string& file, GLTextureOpts opts)
    : mTexture(static_cast<GLuint>(-1)), mOpts(opts) {
  this->Load(file);
}

Peon::GLTexture2D::~GLTexture2D() {
  if (mTexture != static_cast<GLuint>(-1)) {
    glDeleteTextures(1, &mTexture);
  }
}

void Peon::GLTexture2D::Load(const string& file) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* image = stbi_load(file.c_str(), &width, &height, &channels, 0);
  if (!image) {
    LOG_ERROR("Failed to load texture " << file);
  }
  glGenTextures(1, &mTexture);
  glBindTexture(static_cast<GLenum>(mOpts.Target), mTexture);
  glTexParameteri(static_cast<GLenum>(mOpts.Target), GL_TEXTURE_WRAP_S,
                  static_cast<GLint>(mOpts.S));
  glTexParameteri(static_cast<GLenum>(mOpts.Target), GL_TEXTURE_WRAP_T,
                  static_cast<GLint>(mOpts.T));
  glTexParameteri(static_cast<GLenum>(mOpts.Target), GL_TEXTURE_MIN_FILTER,
                  static_cast<GLint>(mOpts.MinFilter));
  glTexParameteri(static_cast<GLenum>(mOpts.Target), GL_TEXTURE_MAG_FILTER,
                  static_cast<GLint>(mOpts.MaxFilter));
  // will have to make this smarter
  if (mOpts.Target == GLTextureTarget::TEXTURE_2D ||
      mOpts.Target == GLTextureTarget::TEXTURE_1D_ARRAY ||
      mOpts.Target == GLTextureTarget::TEXTURE_RECTANGLE) {
    glTexImage2D(static_cast<GLenum>(mOpts.Target), 0,
                 static_cast<GLint>(mOpts.PixelFormat), width, height, 0,
                 static_cast<GLint>(mOpts.PixelFormat), GL_UNSIGNED_BYTE,
                 image);
  }
  if (mOpts.GenerateMipMaps) {
    glGenerateMipmap(static_cast<GLenum>(mOpts.Target));
  }
  stbi_image_free(image);
}

void Peon::GLTexture2D::Bind() {
  glActiveTexture(static_cast<GLenum>(mOpts.TextureUnit));
  glBindTexture(static_cast<GLenum>(mOpts.Target), mTexture);
}

void Peon::GLTexture2D::Unbind() { return; }

void Peon::GLTexture2D::SetTextureUnit(GLTextureUnit unit) {
  this->mOpts.TextureUnit = unit;
}