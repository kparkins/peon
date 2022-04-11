
#include "Sphere.h"

Peon::Shared<Peon::GLVertexArray> Peon::Sphere::MakeSphere(float radius,
                                                           int sectors,
                                                           int stacks) {
  auto bufferData = Sphere::build(radius, sectors, stacks);
  int size = static_cast<int>(bufferData.size() * sizeof(float));
  return GLVertexArray::Create(size, bufferData.data(), GLAttribute3f,
                               GLAttribute3f, GLAttribute2f);
}

void pushVertex(std::vector<float>& dst, std::vector<float>& src, int index) {
  // position
  dst.push_back(src[index]);
  dst.push_back(src[index + 1]);
  dst.push_back(src[index + 2]);
  // normal
  dst.push_back(src[index + 3]);
  dst.push_back(src[index + 4]);
  dst.push_back(src[index + 5]);
  // tex
  dst.push_back(src[index + 6]);
  dst.push_back(src[index + 7]);
}

std::vector<float> Peon::Sphere::build(float radius, int sectors, int stacks) {
  const float PI = static_cast<float>(acos(-1));
  float vx, vy, vz, vxz;
  float nx, ny, nz, lenInverse = 1.0f / radius;
  float s, t;
  float sectorDelta = 2 * PI / sectors;
  float stackDelta = PI / stacks;
  float sectorAngle, stackAngle;

  std::vector<float> data;
  std::vector<float> interleaved;
  data.reserve((stacks + 1) * (sectors + 1) * 8);
  interleaved.reserve(static_cast<int>((data.size() + 1) * 6));

  for (int i = 0; i <= stacks; ++i) {
    stackAngle = PI / 2 - i * stackDelta;
    vxz = radius * cosf(stackAngle);
    vy = radius * sinf(stackAngle);

    for (int j = 0; j <= sectors; ++j) {
      sectorAngle = j * sectorDelta;

      vz = vxz * cosf(sectorAngle);
      vx = vxz * sinf(sectorAngle);
      data.push_back(vx);
      data.push_back(vy);
      data.push_back(vz);

      nx = vx * lenInverse;
      ny = vy * lenInverse;
      nz = vz * lenInverse;

      data.push_back(nx);
      data.push_back(ny);
      data.push_back(nz);

      s = static_cast<float>(j) / sectors;
      t = static_cast<float>(i) / stacks;
      data.push_back(s);
      data.push_back(t);
    }
  }
  unsigned int k1, k2;
  for (int i = 0; i < stacks; ++i) {
    k1 = i * (sectors + 1);
    k2 = k1 + sectors + 1;

    for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
      if (i != 0) {
        pushVertex(interleaved, data, k2 * 8);
        pushVertex(interleaved, data, (k1 + 1) * 8);
        pushVertex(interleaved, data, k1 * 8);
      }

      if (i != (stacks - 1)) {
        pushVertex(interleaved, data, k2 * 8);
        pushVertex(interleaved, data, (k2 + 1) * 8);
        pushVertex(interleaved, data, (k1 + 1) * 8);
      }
    }
  }
  return interleaved;
}