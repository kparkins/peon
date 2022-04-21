#ifndef PEON_FREE_LOOK_CONTROLLER_H
#define PEON_FREE_LOOK_CONTROLLER_H

#include <unordered_map>

#include "event/Bus.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"
#include "graphics/Camera.h"
#include "input/Key.h"

using std::unordered_map;

namespace Peon {

const float CAMERA_DEFAULT_SPEED = 10.f;

class FreelookController {
 public:
  explicit FreelookController();
  explicit FreelookController(const float speed);
  explicit FreelookController(const vec3 position);
  explicit FreelookController(const vec3 position, const float speed);
  virtual ~FreelookController();

  void Stop();
  void Update(float dt);
  void SetSpeed(float speed);
  void SetSensitivity(float sensitivity);
  mat4 GetViewTransform() const;
  vec3 GetPosition() const;
  void SetPosition(const vec3& position);
  vec3 GetLookDirection() const;
  void OnKeyEvent(const KeyEvent& event);
  void OnMouseMove(const MouseMove& event);

 private:
  void UpdateRotation(float dx, float dy);

  float mYaw;
  float mPitch;
  float mSpeed;
  float mSensitivity;
  bool mInitialized;
  Camera mCamera;
  unordered_map<Key, bool> mMoving;
  MouseMove mPrevMouseMove;
};

};  // namespace Peon

#endif