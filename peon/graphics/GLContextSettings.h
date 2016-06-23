#ifndef PEON_GL_CONTEXT_SETTINGS_H
#define PEON_GL_CONTEXT_SETTINGS_H

#include "Peon.h"

class GLContextSettings {
public:

    unsigned int redBitDepth = 8;
    unsigned int greenBitDepth = 8;
    unsigned int blueBitDepth = 8;
    unsigned int alphaBitDepth = 8;
    unsigned int depthBitDepth = 24;
    unsigned int stencilBitDepth = 8;
    unsigned int samples = 0;
    unsigned int contextVersionMajor = 1;
    unsigned int contextVersionMinor = 0;
    unsigned int profile = PEON_OPENGL_ANY_PROFILE;
    bool forwardCompatible = false;
    bool stereoScopic = false;
    bool srgbCapable = false;
    bool doubleBuffer = true;

};

#endif