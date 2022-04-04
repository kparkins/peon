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
  explicit GLProgram(const GLShader& vertex);
  explicit GLProgram(const GLShader& vertex, const GLShader& fragment);
  explicit GLProgram(const GLShader& vertex, const GLShader& geometry,
                     const GLShader& fragment);

  ~GLProgram();

  void Enable();
  void Disable();

  bool IsLinked() const;
  string Error() const;

  void SetUniform(const string& uniform, const mat4& matrix);
  void SetUniform(const string& uniform, const vec4& vector);
  void SetUniform(const string& uniform, const vec3& vector);
  void SetUniform(const string& uniform, float value);
  void SetUniform(const string& uniform, int value);

  GLint GetUniformLocation(const string& uniformName);

 protected:
  bool Link();

  bool mEnabled;
  GLuint mProgram;
  unordered_map<string, GLint> mUniforms;
};
}  // namespace Peon
#endif  // PEON_SHADER_PROGRAM_GL_H
