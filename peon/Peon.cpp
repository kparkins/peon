#include "Peon.h"

bool Peon::Initialize() {
  if (!glfwInit()) {
    return false;
  }
  return true;
}

void Peon::Shutdown() {
  glfwTerminate();
}