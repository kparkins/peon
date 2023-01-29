#include "Plane.h"

Peon::Shared<Peon::GLVertexArray> Peon::Plane::MakePlane(int numSizeTiles,
                                                         float tileSize) {
  auto bufferData = Plane::build(numSizeTiles, tileSize);
  int size = static_cast<int>(bufferData.size() * sizeof(float));
  return GLVertexArray::Create(size, bufferData.data(), GLAttribute3f,
                               GLAttribute3f, GLAttribute2f);
}

void pushVertex(int x,
                int z,
                int numSideTiles,
                std::vector<float>& interleaved) {}

std::vector<float> Peon::Plane::build(int numSideTiles, float tileSize) {
  std::vector<float> vertices;
  std::vector<float> textures;
  std::vector<float> interleaved;

  float sideLength = numSideTiles * tileSize;
  float sideHalfLength = sideLength / 2.f;

  for (float z = -sideHalfLength; z <= sideHalfLength; z += tileSize) {
    for (float x = -sideHalfLength; x <= sideHalfLength; x += tileSize) {
      vertices.push_back(x);
      vertices.push_back(0);
      vertices.push_back(z);
    }
  }

  for (int z = 0; z < numSideTiles; z++) {
    for (int x = 0; x < numSideTiles; x++) {
      int index = z * ((numSideTiles + 1) * 3) + x * 3;
      // vert
      interleaved.push_back(vertices[index]);
      interleaved.push_back(vertices[index + 1]);
      interleaved.push_back(vertices[index + 2]);
      // normal
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
      // tex
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);

      // vert
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 1))]);
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 1)) + 1]);
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 1)) + 2]);
      // normal
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
      // tex
      interleaved.push_back(0.f);
      interleaved.push_back(0.f);

      // vert
      interleaved.push_back(vertices[index + 3]);
      interleaved.push_back(vertices[index + 4]);
      interleaved.push_back(vertices[index + 5]);
      // normal
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
      // tex
      interleaved.push_back(1.f);
      interleaved.push_back(1.f);

      // vert
      interleaved.push_back(vertices[index + 3]);
      interleaved.push_back(vertices[index + 4]);
      interleaved.push_back(vertices[index + 5]);
      // normal
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
      // tex
      interleaved.push_back(1.f);
      interleaved.push_back(1.f);

      // vert
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 1))]);
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 1)) + 1]);
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 1)) + 2]);
      // normal
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
      // tex
      interleaved.push_back(0.f);
      interleaved.push_back(0.f);

      // vert
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 2))]);
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 2)) + 1]);
      interleaved.push_back(vertices[index + (3 * (numSideTiles + 2)) + 2]);
      // normal
      interleaved.push_back(0.f);
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
      // tex
      interleaved.push_back(1.f);
      interleaved.push_back(0.f);
    }
  }

  std::cout << interleaved.size() << std::endl;
  return interleaved;
}