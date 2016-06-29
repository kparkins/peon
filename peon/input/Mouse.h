#ifndef PEON_INPUT_MOUSE_H
#define PEON_INPUT_MOUSE_H

#include <glm/glm.hpp>

#include "graphics/GraphicsExtensions.h"

using glm::ivec2;

namespace Peon {

    class Mouse {

    public:


    protected:

        static void MouseButtonCallback();
        static void CursorPosCallback(GLFWwindow*, double xpos, double ypos);

        friend class GLWindow;

    };

}

#endif