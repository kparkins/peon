#ifndef PEON_GL_SHADER_H
#define PEON_GL_SHADER_H

#include <cassert>
#include <stdexcept>
#include <string>

#include "GraphicsExtensions.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"
#include "common/Utility.h"

using std::runtime_error;
using std::string;

namespace Peon {

enum ShaderType {
  VERTEX = GL_VERTEX_SHADER,
  GEOMETRY = GL_GEOMETRY_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER
};

class GLShader : private Uncopyable {
 public:
  explicit GLShader(ShaderType type);
  ~GLShader();

  void Compile(const string& source);
  void Load(const string& file);
  string Error() const;

  static Shared<GLShader> FromFile(ShaderType type, const string& file);
  static Shared<GLShader> FromSource(ShaderType type, const string& source);

 protected:
  GLuint mId;
  ShaderType mType;
  friend class GLProgram;
};
}  // namespace Peon

#endif