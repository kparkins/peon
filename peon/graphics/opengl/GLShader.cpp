#include "GLShader.h"

Peon::GLShader::GLShader(ShaderType type)
    : mId(glCreateShader(type)), mType(type) {}

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

void Peon::GLShader::Compile(const string& source) {
  GLint success = 0;
  const char* code = source.c_str();
  glShaderSource(mId, 1, &code, 0);
  glCompileShader(mId);
  glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
  if (!success) {
    throw runtime_error(
        StringFormat("Failed to compile shader: %s", this->Error().c_str()));
  }
}

void Peon::GLShader::Load(const string& file) {
  const string& source = Peon::ReadFile(file);
  if (source.empty()) {
    throw runtime_error(StringFormat("Failed to load shader source: %s ",
                                     this->Error().c_str()));
  }
  this->Compile(source);
}

Peon::Shared<Peon::GLShader> Peon::GLShader::FromFile(ShaderType type,
                                                      const string& file) {
  Shared<GLShader> shader = MakeShared<GLShader>(type);
  shader->Load(file);
  return shader;
}

Peon::Shared<Peon::GLShader> Peon::GLShader::FromSource(ShaderType type,
                                                        const string& source) {
  Shared<GLShader> shader = MakeShared<GLShader>(type);
  shader->Compile(source);
  return shader;
}