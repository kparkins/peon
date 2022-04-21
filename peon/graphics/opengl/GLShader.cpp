#include "GLShader.h"

Peon::GLShader::GLShader(ShaderType type)
    : mId(glCreateShader(type)), mType(type) {}

Peon::GLShader::GLShader(ShaderType type, const string& file)
    : mId(glCreateShader(type)), mType(type) {
  this->Load(file);
}

Peon::GLShader::~GLShader() {
  if (static_cast<GLuint>(-1) != mId) {
    glDeleteShader(mId);
  }
}

string Peon::GLShader::Error() const {
  if (mId == static_cast<GLuint>(-1)) {
    return string("");
  }
  GLchar errorLog[512];
  glGetShaderInfoLog(mId, 512, 0, errorLog);
  return string(errorLog);
}

bool Peon::GLShader::Compile(const string& source) {
  GLint success = 0;
  const char* code = source.c_str();
  glShaderSource(mId, 1, &code, 0);
  glCompileShader(mId);
  glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
  if (!success) {
    return false;
  }
  return true;
}

bool Peon::GLShader::Load(const string& file) {
  const string& source = Peon::ReadFile(file);
  return this->Compile(source);
}
