#ifndef PEON_H
#define PEON_H

#include <cstdlib>

#include "log/Logger.h"
#include "graphics/opengl/GraphicsExtensions.h"

namespace Peon {
bool Initialize();
void Shutdown();

#define PEON_INITIALIZE \
  { (void)Peon::Initialize(); }


}  // namespace Peon
#endif