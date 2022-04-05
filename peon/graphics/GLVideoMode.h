//
// Created by Kyle on 5/12/2016.
//

#ifndef PEON_GL_VIDEO_MODE_H
#define PEON_GL_VIDEO_MODE_H

#include <assert.h>

#include "GraphicsExtensions.h"

namespace Peon {

class GLVideoMode {
 public:
  explicit GLVideoMode(unsigned int w = 640, unsigned int h = 480,
                       int rate = PEON_DONT_CARE);
  ~GLVideoMode();

  int refreshRate;
  unsigned int width;
  unsigned int height;
};

}  // namespace Peon

#endif  // PEON_GLVIDEOMODE_H
