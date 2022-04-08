#ifndef PEON_GL_WINDOW_OPTS_H
#define PEON_GL_WINDOW_OPTS_H

#include <string>

#include "GLMonitor.h"
#include "GraphicsExtensions.h"

using std::string;

namespace Peon {
class GLWindowOpts {
 public:
  void Apply() const;

  bool resizeable = true;
  bool visible = true;
  bool decorated = true;
  bool focused = true;
  bool autoIconify = true;
  bool floating = false;
  bool maximized = false;
  ;
  string title = "Default";
};
}  // namespace Peon
#endif