#ifndef PEON_FREE_LOOK_CAMERA_H
#define PEON_FREE_LOOK_CAMERA_H

#include <unordered_map>

#include "Camera.h"
#include "event/KeyListener.h"
#include "event/MouseEvent.h"
#include "input/Key.h"

using std::unordered_map;

namespace Peon {

const float CAMERA_DEFAULT_SPEED = 10.f;

class FreeLookCamera : public EventListener<KeyEvent>,
                       public EventListener<MouseMove>,
                       public Camera {
 public:
  explicit FreeLookCamera();
  explicit FreeLookCamera(const float speed);
  explicit FreeLookCamera(const vec3 position);
  explicit FreeLookCamera(const vec3 position, const float speed);
  ~FreeLookCamera();

  void Stop();
  void Update(float dt);
  void SetSpeed(float speed);
  void SetSensitivity(float sensitivity);

  void UpdateRotation(float dx, float dy);

  virtual void OnKeyEvent(const KeyEvent& event);
  virtual void OnMouseMove(const MouseMove& event);

 private:
  void CalculateVectors();

  float mYaw;
  float mPitch;
  float mSpeed;
  float mSensitivity;
  bool mInitialized;
  unordered_map<Key, bool> mMoving;
  MouseMove mPrevMouseMove;
};

};  // namespace Peon

#endif