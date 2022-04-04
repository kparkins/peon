#ifndef PEON_GL_SURFACE_H
#define PEON_GL_SURFACE_H

namespace Peon {

class GLSurface {
 public:
  GLSurface(int x, int y, size_t w, size_t h) : x(x), y(y), w(w), h(h) {}

  int x;
  int y;
  size_t w;
  size_t h;
};

}  // namespace Peon

#endif