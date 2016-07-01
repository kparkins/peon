#ifndef PEON_H
#define PEON_H

#include <cstdlib>

#include "graphics/GraphicsExtensions.h"

namespace Peon {

    bool Initialize();

}

#define PEON_INITIALIZE { (void)Peon::Initialize(); }

#endif