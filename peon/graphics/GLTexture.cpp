#include "GLTexture.h"

Peon::GLTexture::GLTexture(const std::string& file, GLTextureOpts opts)
    : mTexture(static_cast<GLuint>(-1)), mOpts(opts) {
  this->Load(file);
}

Peon::GLTexture::~GLTexture() {
  if (mTexture != static_cast<GLuint>(-1)) {
    glDeleteTextures(1, &mTexture);
  }
}

void Peon::GLTexture::Load(const string& file) {
  int width, height, channels;
  unsigned char* image = stbi_load(file.c_str(), &width, &height, &channels, 0);
  int pixFmt = (channels > 3) ? GL_RGBA : GL_RGB;
  glGenTextures(1, &mTexture);
  glBindTexture(mOpts.Target, mTexture);
  glTexParameteri(mOpts.Target, GL_TEXTURE_WRAP_S, mOpts.S);
  if (mOpts.Target == TEXTURE_2D) {
    glTexParameteri(mOpts.Target, GL_TEXTURE_WRAP_T, mOpts.T);
  }
  if (mOpts.Target == TEXTURE_3D) {
    glTexParameteri(mOpts.Target, GL_TEXTURE_WRAP_T, mOpts.T);
    glTexParameteri(mOpts.Target, GL_TEXTURE_WRAP_R, mOpts.R);
  }
  glTexParameteri(mOpts.Target, GL_TEXTURE_MIN_FILTER, mOpts.MinFilter);
  glTexParameteri(mOpts.Target, GL_TEXTURE_MAG_FILTER, mOpts.MaxFilter);
  // will have to make this smarter
  if (mOpts.Target == TEXTURE_2D) {
    glTexImage2D(mOpts.Target, 0, pixFmt, width, height, 0, pixFmt,
                 GL_UNSIGNED_BYTE, image);
  }
  if (mOpts.GenerateMipMaps) {
    glGenerateMipmap(mOpts.Target);
  }
  stbi_image_free(image);
}

Peon::Shared<Peon::GLTexture> Peon::GLTexture::FromFile(const string& file,
                                                        GLTextureOpts opts) {
  Shared<GLTexture> texture = MakeShared<GLTexture>(file, opts);
  return texture;
}

void Peon::GLTexture::Bind() {
  glActiveTexture(mOpts.TextureUnit);
  glBindTexture(mOpts.Target, mTexture);
}

void Peon::GLTexture::Unbind() { return; }

void Peon::GLTexture::SetTextureUnit(GLTextureUnit unit) {
  this->mOpts.TextureUnit = unit;
}