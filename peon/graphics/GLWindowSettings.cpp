#include "GLWindowSettings.h"

void Peon::GLWindowSettings::ApplySettings() const {
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    glfwWindowHint(GLFW_VISIBLE, visible);
    glfwWindowHint(GLFW_DECORATED, decorated);
    glfwWindowHint(GLFW_FOCUSED, focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY, autoIconify);
    glfwWindowHint(GLFW_FLOATING, floating);
    glfwWindowHint(GLFW_MAXIMIZED, maximized);
}
