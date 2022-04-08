#ifndef PEON_FREE_LOOK_CAMERA_H
#define PEON_FREE_LOOK_CAMERA_H

#include <unordered_map>

#include "Camera.h"
#include "event/KeyListener.h"
#include "event/MouseEvent.h"
#include "input/Key.h"

using std::unordered_map;

namespace Peon {
class FreeLookCamera : public EventListener<KeyEvent>,
                       public EventListener<MouseMove> {
 public:
  FreeLookCamera() : mPrevMouseMove(MouseMove(320, 240)), mSpeed(10.f) {
    mMoving[Key::W] = false;
    mMoving[Key::A] = false;
    mMoving[Key::S] = false;
    mMoving[Key::D] = false;
  }

  ~FreeLookCamera() {}

  void Update(float dt) {
    if (mMoving[Key::W]) {
      mCamera.position += mCamera.front * mSpeed * dt;
    }
    if (mMoving[Key::S]) {
      mCamera.position -= mCamera.front * mSpeed * dt;
    }
    if (mMoving[Key::A]) {
      mCamera.position -=
          normalize(cross(mCamera.front, mCamera.up)) * mSpeed * dt;
    }
    if (mMoving[Key::D]) {
      mCamera.position +=
          normalize(cross(mCamera.front, mCamera.up)) * mSpeed * dt;
    }
  }

  virtual void OnKeyEvent(const KeyEvent& event) {
    if (event.action == KeyAction::REPEAT) {
      return;
    }
    bool value = event.action == KeyAction::PRESS ? true : false;
    mMoving[event.key] = value;
  }

  virtual void OnMouseMove(const MouseMove& event) {
    float dx = event.x - mPrevMouseMove.x;
    float dy = event.y - mPrevMouseMove.y;
    mPrevMouseMove = event;
    mCamera.UpdateRotation(dx, dy);
  }

  mat4 GetViewTransform() { return mCamera.GetViewTransform(); }

 private:
  float mSpeed;
  unordered_map<Key, bool> mMoving;
  MouseMove mPrevMouseMove;
  Camera mCamera;
};

};  // namespace Peon

#endif