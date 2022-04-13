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
#include "Physics.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "event/Event.h"
#include "event/EventDispatcher.h"
#include "event/KeyListener.h"
#include "event/MouseEvent.h"
#include "event/WindowListener.h"
#include "graphics/FreelookCamera.h"
#include "graphics/Sphere.h"
#include "graphics/opengl/GLContext.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLTexture2D.h"
#include "graphics/opengl/GLVertexArray.h"
#include "graphics/opengl/GLWindow.h"
#include "input/Key.h"
#include "log/Logger.h"
#include "log/StdoutStream.h"

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
typedef struct Model {
  Model(Shared<GLVertexArray> buffer, Shared<GLProgram> program = nullptr)
      : buffer(buffer), program(program) {}

  void Draw() {
    buffer->Bind();
    buffer->Draw();
    buffer->Unbind();
  }

  Shared<GLProgram> program;
  Shared<GLTexture2D> texture;
  Shared<GLVertexArray> buffer;
} Model;

typedef struct GameObject {
  GameObject(Shared<Model> model) : model(model), matrix(mat4(1.f)) {}
  GameObject(Shared<GLVertexArray> buffer, Shared<GLProgram> program)
      : model(MakeShared<Model>(buffer, program)), matrix(mat4(1.f)) {}

  Shared<Model> model;
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

class Game : EventListener<KeyEvent> {
 public:
  Game(Shared<GLWindow> window) : window(window) {
    physics = new PhysicsEngine();
  }

  ~Game() {
    delete physics;

    sphere.reset();
    cube.reset();
    light.reset();
    lightShader.reset();
    lightingShader.reset();
    freecam.reset();
    window.reset();
    renderer.reset();
  }

  virtual void OnKeyEvent(const KeyEvent& event) {
    if (event.action == KeyAction::PRESS && event.key == Key::SPACE) {
      vec3 d = freecam->GetLookDirection();
      btVector3 direction(d.x, d.y, d.z);
      vec3 position = freecam->GetPosition();
      auto s = physics->AddSphere(0.10795f, position, 0.625f);
      s->setRestitution(.75f);
      s->applyCentralImpulse(direction * 5.f);
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

    Scene* scene = new Scene();
    EntityId id = scene->CreateEntity();
    scene->AddComponent<Transform>(id);
    Transform* t = scene->GetComponent<Transform>(id);
    t->position = vec3(1, 2, 3);
    Transform* t2 = scene->GetComponent<Transform>(id);
    scene->RemoveComponent<Transform>(id);
    Transform* t3 = scene->GetComponent<Transform>(id);
    assert(t3 == nullptr);
    scene->DestroyEntity(id);
    EntityId newId = scene->CreateEntity();
    scene->DestroyEntity(newId);
    delete scene;

    freecam = MakeShared<FreeLookCamera>();
    freecam->SetPosition(vec3(0.f, 0.5f, 4.f));

    window->SetCursorMode(CursorMode::DISABLED);
    renderer->SetViewport(window->GetViewport());

    /*auto cubeBuffer =
        GLVertexArray::Create(sizeof(cubeVertices), cubeVertices,
        GLAttribute3f,
                               GLAttribute3f, GLAttribute2f);*/
    auto lightBuffer = Sphere::MakeSphere(.1f, 20, 10);
    auto sphereBuffer = Sphere::MakeSphere(.10795f, 100, 50);

    //  cube = MakeShared<GameObject>(cubeBuffer, lightingShader);
    light = MakeShared<GameObject>(lightBuffer, lightShader);

    sphere = MakeShared<GameObject>(sphereBuffer, lightingShader);
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
      physics->StepSimulation(dt);

      mat4 view = freecam->GetViewTransform();

      // mat3 cubeNormalMatrix = mat3(transpose(inverse(view * cube->matrix)));
      /*
             auto cubeShader = cube->model->program;
             cubeShader->Enable();
             cubeShader->SetUniform("view", view);
             cubeShader->SetUniform("model", cube->matrix);
             cubeShader->SetUniform("projection", projection);
             cubeShader->SetUniform("objectColor", vec3(1.0f, 0.5f, 0.31f));
             cubeShader->SetUniform("lightColor", vec3(1.0, 0.9803, 0.8039));
             cubeShader->SetUniform("lightPosition", spherePosition);
             cubeShader->SetUniform("normalMatrix", cubeNormalMatrix);
             cube->model->Draw(cube->matrix);*/

      vec3 unit = vec3(2 * cos(currentFrame), 1.f, 2 * sin(currentFrame));
      light->matrix = translate(mat4(1.f), unit);
      vec4 lightPosition = light->matrix * vec4(0.f, 0.f, 0.f, 1.f);
      for (auto rigidBody : physics->GetRigidBodies()) {
        mat4 matrix(0.f);
        btTransform sphereTransform = rigidBody->getWorldTransform();

        sphereTransform.getOpenGLMatrix(value_ptr(matrix));

        auto sphereShader = sphere->model->program;
        mat3 sphereNormalMatrix = mat3(transpose(inverse(view * matrix)));
        sphereShader->Enable();
        sphereShader->SetUniform("view", view);
        sphereShader->SetUniform("model", matrix);
        sphereShader->SetUniform("projection", projection);
        sphereShader->SetUniform("objectColor", vec3(1.0f, 0.5f, 0.31f));
        sphereShader->SetUniform("lightColor", vec3(1.0, 0.9803, 0.8039));
        sphereShader->SetUniform("lightPosition", lightPosition);
        sphereShader->SetUniform("normalMatrix", sphereNormalMatrix);
        sphere->model->Draw();
      }

      mat4 lightModelView = view * light->matrix;
      auto lightProgram = light->model->program;
      lightProgram->Enable();
      lightProgram->SetUniform("modelView", lightModelView);
      lightProgram->SetUniform("projection", projection);
      light->model->Draw();

      window->SwapBuffers();
    }
  }

 private:
  PhysicsEngine* physics;
  Shared<GameObject> sphere;
  Shared<GameObject> cube;
  Shared<GameObject> light;
  Shared<GLProgram> lightShader;
  Shared<GLProgram> lightingShader;
  Shared<FreeLookCamera> freecam;
  Shared<GLWindow> window;
  Shared<GLRenderer> renderer;
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

  Game g(window);
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