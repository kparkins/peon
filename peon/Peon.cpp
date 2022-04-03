#include "Peon.h"

bool Peon::Initialize() {
    if (!glfwInit()) {
        return false;
    }
    atexit(glfwTerminate);
    return true;
}
