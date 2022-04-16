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
  explicit Camera();
  explicit Camera(vec3 position, vec3 front, vec3 up,
                  vec3 worldUp = vec3(0.f, 1.f, 0.f));
  virtual ~Camera();

  void SetPosition(const vec3 pos);
  vec3 GetPosition() const;
  mat4 GetViewTransform() const;
  void SetLookDirection(const vec3& direction);
  vec3 GetLookDirection() const;
  void SetUp(const vec3& up);
  vec3 GetUp() const;

 protected:
  vec3 mPosition;
  vec3 mFront;
  vec3 mUp;
  vec3 mWorldUp;
};

}  // namespace Peon

#endif