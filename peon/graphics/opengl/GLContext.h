//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_CONTEXT_H
#define PEON_GL_CONTEXT_H

#include "GLContextOpts.h"
#include "GLProgram.h"
#include "GLVideoMode.h"
#include "GLWindowOpts.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"
#include "log/Logger.h"

namespace Peon {

typedef void (*GLProcAddress)(void);

class GLContext : private Uncopyable {
 public:
  explicit GLContext(const GLContext* const partner);
  explicit GLContext(const GLContextOpts& settings = GLContextOpts());

  ~GLContext();

  GLProcAddress IsExtensionSupported(const string& extensionName);

  void MakeContextCurrent();
  bool IsContextCurrent();

 private:
  GLContext(const GLContextOpts& contextOps,
            const GLVideoMode& videoMode,
            const GLWindowOpts& windowOpts,
            GLFWwindow* shared = nullptr);

  void Apply();
  static void error(int num, const char* msg);

  GLFWwindow* mWindow;
  GLContextOpts mOpts;

  friend class GLWindow;
};
}  // namespace Peon

#endif  // PEON_GL_RENDER_CONTEXT_H
