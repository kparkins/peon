#include "FreeLookCamera.h"

Peon::FreeLookCamera::FreeLookCamera()
    : mPrevMouseMove(MouseMove(0, 0)),
      mSpeed(CAMERA_DEFAULT_SPEED),
      mInitialized(false),
      mSensitivity(0.1f),
      mPitch(0.f),
      mYaw(-90.f) {
  this->Stop();
}

Peon::FreeLookCamera::FreeLookCamera(const float speed)
    : mPrevMouseMove(MouseMove(0, 0)),
      mSpeed(speed),
      mInitialized(false),
      mSensitivity(0.1f),
      mPitch(0.f),
      mYaw(-90.f) {
  this->Stop();
}

Peon::FreeLookCamera::FreeLookCamera(const vec3 postion)
    : mPrevMouseMove(MouseMove(0, 0)),
      mSpeed(CAMERA_DEFAULT_SPEED),
      mInitialized(false),
      mSensitivity(0.1f),
      mPitch(0.f),
      mYaw(-90.f) {
  this->Stop();
  this->mPosition = postion;
}

Peon::FreeLookCamera::FreeLookCamera(const vec3 position, const float speed)
    : mPrevMouseMove(MouseMove(0, 0)),
      mSpeed(speed),
      mInitialized(false),
      mSensitivity(0.1f),
      mPitch(0.f),
      mYaw(-90.f) {
  this->Stop();
  this->mPosition = position;
}

Peon::FreeLookCamera::~FreeLookCamera() {}

void Peon::FreeLookCamera::SetSpeed(float speed) { this->mSpeed = speed; }

void Peon::FreeLookCamera::SetSensitivity(float sensitivity) {
  this->mSensitivity = sensitivity;
}

void Peon::FreeLookCamera::Update(float dt) {
  if (mMoving[Key::W]) {
    mPosition += mFront * mSpeed * dt;
  }
  if (mMoving[Key::S]) {
    mPosition -= mFront * mSpeed * dt;
  }
  if (mMoving[Key::A]) {
    mPosition -= normalize(cross(mFront, mUp)) * mSpeed * dt;
  }
  if (mMoving[Key::D]) {
    mPosition += normalize(cross(mFront, mUp)) * mSpeed * dt;
  }
}

void Peon::FreeLookCamera::UpdateRotation(float dx, float dy) {
  this->mYaw += dx * mSensitivity;
  this->mPitch += -dy * mSensitivity;
  if (this->mPitch > 89.f) {
    this->mPitch = 89.f;
  } else if (this->mPitch < -89.f) {
    this->mPitch = -89.f;
  }
  this->CalculateVectors();
}

void Peon::FreeLookCamera::CalculateVectors() {
  vec3 newFront;
  newFront.x = cos(radians(mYaw)) * cos(radians(mPitch));
  newFront.y = sin(radians(mPitch));
  newFront.z = sin(radians(mYaw)) * cos(radians(mPitch));
  this->mFront = normalize(newFront);
  vec3 right = normalize(cross(mFront, mWorldUp));
  this->mUp = normalize(cross(right, mFront));
}

void Peon::FreeLookCamera::Stop() {
  mMoving[Key::W] = false;
  mMoving[Key::A] = false;
  mMoving[Key::S] = false;
  mMoving[Key::D] = false;
}

void Peon::FreeLookCamera::OnKeyEvent(const KeyEvent& event) {
  if (event.action == KeyAction::REPEAT) {
    return;
  }
  mMoving[event.key] = event.action == KeyAction::PRESS;
}

void Peon::FreeLookCamera::OnMouseMove(const MouseMove& event) {
  if (!this->mInitialized) {
    mPrevMouseMove = event;
    this->mInitialized = true;
  }
  float dx = event.x - mPrevMouseMove.x;
  float dy = event.y - mPrevMouseMove.y;
  mPrevMouseMove = event;
  UpdateRotation(dx, dy);
}
