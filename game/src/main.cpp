/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "Entity.h"
#include "Peon.h"
#include "Scene.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "event/Bus.h"
#include "event/MouseEvent.h"
#include "graphics/Sphere.h"
#include "graphics/opengl/GLContext.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLTexture2D.h"
#include "graphics/opengl/GLVertexArray.h"
#include "graphics/opengl/GLWindow.h"
#include "input/FreelookCamera.h"
#include "input/Key.h"
#include "log/Logger.h"
#include "log/StdoutStream.h"
#include "physics/PhysicsSystem.h"

using std::cerr;
using std::cout;
using std::endl;
using std::unordered_map;

using namespace Peon;
using namespace glm;

static float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

namespace Peon {

typedef struct GameObject {
  GameObject(Shared<GLVertexArray> buffer, Shared<GLProgram> program)
      : buffer(buffer), program(program), matrix(mat4(1.f)) {}

  void Draw() {
    buffer->Bind();
    buffer->Draw();
    buffer->Unbind();
  }

  Shared<GLProgram> program;
  Shared<GLVertexArray> buffer;
  mat4 matrix;
} GameObject;

class GLRenderer {
 public:
  GLRenderer() {}

  ~GLRenderer() {}

  void SetViewport(const GLViewport& view) {
    glViewport(view.x, view.y, static_cast<GLsizei>(view.w),
               static_cast<GLsizei>(view.h));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
  }

  void Clear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  };
};

class Game {
 public:
  Game(Shared<GLWindow> window, Shared<Bus> bus) : window(window), bus(bus) {
    scene = MakeShared<Scene>();
    physics = MakeShared<PhysicsSystem>(scene, bus);
  }

  ~Game() {}

  virtual void OnKeyEvent(const KeyEvent& event) {
    if (event.action != KeyAction::PRESS) {
      return;
    }
    vec3 direction = freecam->GetLookDirection();
    vec3 position = freecam->GetPosition();
    if (event.key == Key::SPACE) {
      Entity* entity = scene->CreateEntity();
      auto transform = entity->GetComponent<Transform>();
      transform->matrix = translate(transform->matrix, position);
      auto body =
          entity->AddComponent<RigidBody>(1.f, CollisionShape::Sphere(.10795f));
      body->SetRestitution(.75f);
      body->SetRollingFriction(.3f);
      body->SetFriction(.5f);
      body->ApplyCentralImpulse(direction * 10.f);
      physics->AddRigidBody(body);
      entity->AddComponent<GameObject>(sphereBuffer, lightingShader);
    } else if (event.key == Key::LEFT_CONTROL) {
      Entity* entity = scene->CreateEntity();
      entity->AddComponent<GameObject>(cubeBuffer, lightingShader);

      auto transform = entity->GetComponent<Transform>();
      transform->matrix = translate(transform->matrix, position);

      CollisionShape shape = CollisionShape::Box(vec3(.5f, .5f, .5f));
      auto body = entity->AddComponent<RigidBody>(3.f, shape);

      body->ApplyCentralImpulse(direction * 30.f);
      body->SetRestitution(.2f);
      body->SetFriction(.8f);
      physics->AddRigidBody(body);
    }
  }

  void Initialize() {
    GLShader vertex(ShaderType::VERTEX);
    GLShader fragment(ShaderType::FRAGMENT);
    vertex.Load("res/shaders/Lighting.vert");
    fragment.Load("res/shaders/Lighting.frag");
    lightingShader = MakeShared<GLProgram>(vertex, fragment);

    GLShader lightVertex(ShaderType::VERTEX);
    GLShader lightFragment(ShaderType::FRAGMENT);
    lightVertex.Load("res/shaders/LightSource.vert");
    lightFragment.Load("res/shaders/LightSource.frag");
    lightShader = MakeShared<GLProgram>(lightVertex, lightFragment);

    freecam = MakeShared<FreeLookCamera>();
    freecam->SetPosition(vec3(0.f, 0.5f, 4.f));

    bus->Connect<KeyEvent>(freecam, &FreeLookCamera::OnKeyEvent);
    bus->Connect<MouseMove>(freecam, &FreeLookCamera::OnMouseMove);
    bus->Connect<KeyEvent>(this, &Game::OnKeyEvent);

    window->SetCursorMode(CursorMode::DISABLED);
    renderer->SetViewport(window->GetViewport());

    cubeBuffer =
        GLVertexArray::Create(sizeof(cubeVertices), cubeVertices, GLAttribute3f,
                              GLAttribute3f, GLAttribute2f);

    auto lightBuffer = Sphere::MakeSphere(.1f, 20, 10);
    sphereBuffer = Sphere::MakeSphere(.10795f, 100, 50);

    light = MakeShared<GameObject>(lightBuffer, lightShader);
    this->MakeGround();
  }

  void MakeGround() {
    Entity* entity = scene->CreateEntity();
    CollisionShape shape(
        new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0)));
    auto ground = entity->AddComponent<RigidBody>(0.f, shape);
    ground->SetCollisionShape(shape);
    ground->SetRollingFriction(.3f);
    ground->SetFriction(.9f);
    ground->SetRestitution(1.f);
    physics->AddRigidBody(ground);
  }

  void Run() {
    mat4 projection = perspective(radians(65.f), 1024.f / 576.f, 0.2f, 3500.f);
    float prevFrame = static_cast<float>(glfwGetTime());

    while (window->IsOpen()) {
      float currentFrame = static_cast<float>(glfwGetTime());
      float dt = currentFrame - prevFrame;
      prevFrame = currentFrame;
      renderer->Clear();
      freecam->Update(dt);
      physics->Update(dt);

      mat4 view = freecam->GetViewTransform();

      vec3 unit = vec3(2 * cos(currentFrame), 1.f, 2 * sin(currentFrame));
      light->matrix = translate(mat4(1.f), unit);
      vec4 lightPosition = light->matrix * vec4(0.f, 0.f, 0.f, 1.f);

      for (auto entity : scene->GetEntitiesWith<GameObject>()) {
        auto transform = entity->GetComponent<Transform>();
        auto object = entity->GetComponent<GameObject>();
        auto shader = object->program;
        mat3 objectNormalMatrix =
            mat3(transpose(inverse(view * transform->matrix)));
        shader->Enable();
        shader->SetUniform("view", view);
        shader->SetUniform("model", transform->matrix);
        shader->SetUniform("projection", projection);
        shader->SetUniform("objectColor", vec3(1.0f, 0.5f, 0.31f));
        shader->SetUniform("lightColor", vec3(1.0, 0.9803, 0.8039));
        shader->SetUniform("lightPosition", lightPosition);
        shader->SetUniform("normalMatrix", objectNormalMatrix);
        object->Draw();
      }

      mat4 lightModelView = view * light->matrix;
      auto lightProgram = light->program;
      lightProgram->Enable();
      lightProgram->SetUniform("modelView", lightModelView);
      lightProgram->SetUniform("projection", projection);
      light->Draw();

      window->SwapBuffers();
    }
  }

 private:
  Shared<GameObject> light;
  Shared<GLProgram> lightShader;
  Shared<GLProgram> lightingShader;
  Shared<GLVertexArray> sphereBuffer;
  Shared<GLVertexArray> cubeBuffer;
  Shared<PhysicsSystem> physics;

  Shared<Bus> bus;
  Shared<Scene> scene;
  Shared<GLWindow> window;
  Shared<GLRenderer> renderer;
  Shared<FreeLookCamera> freecam;
};
}  // namespace Peon

int main(int argc, char* argv[]) {
  Peon::Initialize();
  gLogger.AddStream(MakeUnique<StdoutStream>());
  gLogger.SetLogLevel(LogLevel::TRACE);

  GLContextOpts ctxOpts;
  ctxOpts.forwardCompatible = false;
  ctxOpts.profile = PEON_OPENGL_CORE_PROFILE;
  ctxOpts.versionMajor = 4;
  ctxOpts.versionMinor = 0;
  ctxOpts.samples = 8;

  GLVideoMode videoMode(1024, 576);

  auto window = MakeShared<GLWindow>(ctxOpts, videoMode);

  Game g(window, window->GetDefaultBus());
  try {
    g.Initialize();
    g.Run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    Peon::Shutdown();
    return EXIT_FAILURE;
  }

  Peon::Shutdown();
  return 0;
}