//
// Created by Kyle on 5/12/2016.
//

#ifndef PEON_GL_VIDEO_MODE_H
#define PEON_GL_VIDEO_MODE_H


#include <GLFW/glfw3.h>
#include <assert.h>

#include "common/Macros.h"

namespace Peon {
    class GLVideoMode {
    public:
        explicit GLVideoMode(unsigned int w = 640, unsigned int h = 480, unsigned int rate = PEON_DONT_CARE);
        ~GLVideoMode();

        unsigned int refreshRate;
        unsigned int width;
        unsigned int height;

    private:

        explicit GLVideoMode(GLFWvidmode* mode);

        friend class GLMonitor;

    };
}

#endif //PEON_GLVIDEOMODE_H
