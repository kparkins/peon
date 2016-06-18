//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_CONTEXT_H
#define PEON_GL_CONTEXT_H

#include <GLFW/glfw3.h>

#include "log/Logger.h"
#include "common/Uncopyable.h"

namespace Peon {

   
    class GLContext : private Uncopyable {

    public:

        GLContext();

        ~GLContext();


    private:

        GLFWwindow* mWindow;

    };
}


#endif //PEON_GL_RENDER_CONTEXT_H
