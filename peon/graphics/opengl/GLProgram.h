/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_GL_PROGRAM_H
#define PEON_GL_PROGRAM_H

#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "GLShader.h"
#include "GraphicsExtensions.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"
#include "log/Logger.h"

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

using namespace glm;

namespace Peon {

class GLProgram : private Uncopyable {
 public:
  explicit GLProgram(const GLShader& first, const GLShader& second);
  explicit GLProgram(vector<GLShader*>& stages);

  ~GLProgram();

  inline void Enable();
  inline void Disable();

  inline bool IsLinked() const;
  inline string Error() const;

  inline void SetUniform(const string& uniform, const mat4& matrix);
  inline void SetUniform(const string& uniform, const mat3& matrix);
  inline void SetUniform(const string& uniform, const vec4& vector);
  inline void SetUniform(const string& uniform, const vec3& vector);
  inline void SetUniform(const string& uniform, float value);
  inline void SetUniform(const string& uniform, int value);

  GLint GetUniformLocation(const string& uniformName);

 protected:
  bool Link();
  void SetupUniforms();

  bool mEnabled;
  GLuint mProgram;
  unordered_map<string, GLint> mUniforms;
};

std::string Peon::GLProgram::Error() const {
  GLint logLength = 0;
  glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
  char* errorLog = new char[logLength];
  std::memset(errorLog, 0, logLength);
  glGetProgramInfoLog(mProgram, logLength, 0, errorLog);
  string error = std::string(errorLog);
  delete errorLog;
  return error;
}

void Peon::GLProgram::Enable() {
  assert(this->IsLinked());
  glUseProgram(mProgram);
  mEnabled = true;
}

void Peon::GLProgram::Disable() {
  assert(this->IsLinked());
  assert(mEnabled);
  glUseProgram(0);
  mEnabled = false;
}

bool Peon::GLProgram::IsLinked() const {
  return this->mProgram != static_cast<GLuint>(-1);
}

void Peon::GLProgram::SetUniform(const string& uniform, const mat4& matrix) {
  auto it = mUniforms.find(uniform);
  // assert(mUniforms.find(uniform) != mUniforms.end());
  if (mUniforms.find(uniform) == mUniforms.end()) {
    std::cout << "wtf" << std::endl;
  }
  glUniformMatrix4fv(mUniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Peon::GLProgram::SetUniform(const string& uniform, const mat3& matrix) {
  assert(mUniforms.find(uniform) != mUniforms.end());
  glUniformMatrix3fv(mUniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Peon::GLProgram::SetUniform(const string& uniform, const vec3& vector) {
  assert(mUniforms.find(uniform) != mUniforms.end());
  glUniform3fv(mUniforms[uniform], 1, glm::value_ptr(vector));
}

void Peon::GLProgram::SetUniform(const string& uniform, const vec4& vector) {
  assert(mUniforms.find(uniform) != mUniforms.end());
  glUniform4fv(mUniforms[uniform], 1, glm::value_ptr(vector));
}

void Peon::GLProgram::SetUniform(const string& uniform, float value) {
  assert(mUniforms.find(uniform) != mUniforms.end());
  glUniform1f(mUniforms[uniform], value);
}

void Peon::GLProgram::SetUniform(const string& uniform, int value) {
  assert(mUniforms.find(uniform) != mUniforms.end());
  glUniform1i(mUniforms[uniform], value);
}

}  // namespace Peon
#endif  // PEON_SHADER_PROGRAM_GL_H
