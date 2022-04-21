#ifndef PEON_GL_SHADER_H
#define PEON_GL_SHADER_H

#include <cassert>
#include <stdexcept>
#include <string>

#include "GraphicsExtensions.h"
#include "common/Filesystem.h"
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
  explicit GLShader(ShaderType type, const string& file);
  ~GLShader();

  bool Compile(const string& source);
  bool Load(const string& file);
  string Error() const;

 protected:
  GLuint mId;
  ShaderType mType;
  friend class GLProgram;
};
}  // namespace Peon

#endif