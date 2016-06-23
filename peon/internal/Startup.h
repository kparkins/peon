#ifndef PEON_INTERNAL_STARTUP_H
#define PEON_INTERNAL_STARTUP_H

#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>

namespace Peon {
    namespace internal {
        class Startup {
        public:

            Startup() {
                
                if (!glfwInit()) {
                    exit(EXIT_FAILURE);
                }
                std::cout << "glfw init" << std::endl;
            }

            ~Startup() {
                glfwTerminate();
                std::cout << "glfw close" << std::endl;
            }
        };

        
    }
} 

#endif

