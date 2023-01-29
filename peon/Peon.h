#ifndef PEON_H
#define PEON_H

#include <cstdlib>

#include "graphics/opengl/GraphicsExtensions.h"
#include "log/Logger.h"

namespace Peon {
bool Initialize();
void Shutdown();

#define PEON_INITIALIZE \
  { (void)Peon::Initialize(); }

}  // namespace Peon
#endif