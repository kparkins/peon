#include "Mouse.h"

#include <iostream>
void Peon::Mouse::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << " x - " << xpos << " y - " << ypos << std::endl;
}