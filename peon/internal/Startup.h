#ifndef PEON_INTERNAL_STARTUP_H
#define PEON_INTERNAL_STARTUP_H

#include <cstdlib>

#include <GLFW/glfw3.h>

namespace Peon {
    namespace internal {
        class Startup {
        public:

            Startup() {
                if (!glfwInit()) {
                    exit(EXIT_FAILURE);
                }
            }

            ~Startup() {
                glfwTerminate();
            }
        };

        Startup startup;
    }
} 

#endif

