#include "Camera.h"

Peon::Camera::Camera()
    : mPosition(vec3(0.f, 0.f, 3.f)),
      mFront(vec3(0.f, 0.f, -3.f)),
      mUp(vec3(0.f, 1.f, 0.f)),
      mWorldUp(vec3(0.f, 1.f, 0.f)) {}

Peon::Camera::Camera(vec3 position, vec3 front, vec3 up, vec3 worldUp)
    : mPosition(vec3(0.f, 0.f, 3.f)),
      mFront(vec3(0.f, 0.f, -3.f)),
      mUp(vec3(0.f, 1.f, 0.f)),
      mWorldUp(worldUp) {}

Peon::Camera::~Camera() {}

void Peon::Camera::SetPosition(const vec3 pos) { this->mPosition = pos; }

vec3 Peon::Camera::GetPosition() const { return this->mPosition; }

mat4 Peon::Camera::GetViewTransform() {
  return lookAt(mPosition, mPosition + mFront, mUp);
}

vec3 Peon::Camera::GetLookDirection() const { return normalize(this->mFront); }
