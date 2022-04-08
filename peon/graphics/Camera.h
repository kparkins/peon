#ifndef PEON_CAMERA_H
#define PEON_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::lookAt;
using glm::mat4;
using glm::radians;
using glm::vec3;

namespace Peon {
class Camera {
 public:
  Camera() {
    position = vec3(0.f, 0.f, 3.0f);
    front = vec3(0.f, 0.f, -3.0f);
    up = vec3(0.f, 1.f, 0.f);
    pitch = 0.f;
    yaw = -90.f;
    worldUp = vec3(0.f, 1.f, 0.f);
  }

  mat4 GetViewTransform() { return lookAt(position, position + front, up); }

  void UpdateRotation(float dx, float dy) {
    this->yaw += dx * sensitivity;
    this->pitch += -dy * sensitivity;
    if (this->pitch > 89.f) {
      this->pitch = 89.f;
    } else if (this->pitch < -89.f) {
      this->pitch = -89.f;
    }
    this->CalculateVectors();
  }

  void CalculateVectors() {
    vec3 newFront;
    newFront.x = cos(radians(yaw)) * cos(radians(pitch));
    newFront.y = sin(radians(pitch));
    newFront.z = sin(radians(yaw)) * cos(radians(pitch));
    this->front = normalize(newFront);
    vec3 right = normalize(cross(front, worldUp));
    this->up = normalize(cross(right, front));
  }

  const float sensitivity = 0.1f;
  float yaw;
  float pitch;
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 worldUp;
};

}  // namespace Peon

#endif