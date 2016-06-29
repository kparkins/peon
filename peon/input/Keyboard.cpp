#include "Keyboard.h"
#include <iostream>
#include "graphics/GLWindow.h"

unordered_map<Peon::Keyboard::Key, bool> Peon::Keyboard::mKeyPressed;

Peon::Keyboard::Keyboard() {
    mKeyPressed[Key::UNKNOWN] = false;
    for (int i = Key::SPACE; i < Key::MENU; ++i) {
        mKeyPressed[static_cast<Key>(i)] = false;
    }
}

bool Peon::Keyboard::IsKeyPressed(Key k) {
    return mKeyPressed[k];
}

void Peon::Keyboard::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
    mKeyPressed[static_cast<Key>(key)] = (action == GLFW_PRESS);
    
    GLWindow* window = reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(windowHandle));
    if (key == Key::ESCAPE) {
        window->CloseWindow();
    }
}