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
#include "Loaders.h"
#include "Peon.h"
#include "Scene.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "event/Bus.h"
#include "event/MouseEvent.h"
#include "graphics/Plane.h"
#include "graphics/Sphere.h"
#include "graphics/opengl/GLContext.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLTexture2D.h"
#include "graphics/opengl/GLVertexArray.h"
#include "graphics/opengl/GLWindow.h"
#include "input/FreelookController.h"
#include "input/Key.h"
#include "log/Logger.h"
#include "log/StdoutStream.h"
#include "physics/PhysicsSystem.h"
#include "profile/BlockTimer.h"

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

static float quadVertices[] = {
    -0.5f, 0,   -0.5f, 0.f, 1.f,  0.f, 0.f,   1.f, -0.5f, 0,   0.5f, 0.f,
    1.f,   0.f, 0.f,   0.f, 0.5f, 0,   -0.5f, 0.f, 1.f,   0.f, 1.f,  1.f,

    0.5f,  0,   -0.5f, 0.f, 1.f,  0.f, 1.f,   1.f, -0.5f, 0,   0.5f, 0.f,
    1.f,   0.f, 0.f,   0.f, 0.5f, 0,   0.5f,  0.f, 1.f,   0.f, 1.f,  0.f,
};

namespace Peon {

typedef struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
} Material;

typedef struct LightMap {
  vec3 ambient;
  GLTexture2D* diffuse;
  GLTexture2D* specular;
  float shininess;
} LightMap;

typedef struct Model {
  Model(Shared<GLVertexArray> buffer, GLProgram* program)
      : buffer(buffer), program(program), texture(nullptr), map(nullptr) {}

  void Draw() {
    if (texture) {
      texture->Bind();
    }
    if (map) {
      map->diffuse->SetTextureUnit(GLTextureUnit::TEXTURE0);
      map->diffuse->Bind();
      map->specular->SetTextureUnit(GLTextureUnit::TEXTURE1);
      map->specular->Bind();
      program->Enable();
      program->SetUniform("objectMap.diffuse", 0);
      program->SetUniform("objectMap.specular", 1);
      program->SetUniform("objectMap.shininess", map->shininess);
    }
    buffer->Bind();
    buffer->Draw();
    buffer->Unbind();
    program->Disable();
  }

  GLProgram* program;
  GLTexture2D* texture;
  LightMap* map;
  Shared<GLVertexArray> buffer;
  mat4 matrix;
} Model;

class GLRenderer {
 public:
  GLRenderer() {}

  ~GLRenderer() {}

  void SetViewport(const GLViewport& view) {
    glViewport(view.x, view.y, static_cast<GLsizei>(view.w),
               static_cast<GLsizei>(view.h));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
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
      auto body = entity->AddComponent<RigidBody>(
          1.f, move(CollisionShapes::NewSphere(.10795f)));
      body->SetRestitution(.75f);
      body->SetRollingFriction(.002f);
      body->SetSpinningFriction(.02f);
      body->SetFriction(.8f);
      body->ApplyCentralImpulse(direction * 7.f);
      physics->SyncTransform(body);
      physics->AddRigidBody(body);
      auto model = entity->AddComponent<Model>(sphereBuffer,
                                               shaders->Lookup("Lighting"));
      model->texture = textures->Lookup("ball");
      model->map = nullptr;

    } else if (event.key == Key::LEFT_CONTROL) {
      Entity* entity = scene->CreateEntity();
      auto model = entity->AddComponent<Model>(cubeBuffer,
                                               shaders->Lookup("bp-light-map"));
      model->texture = nullptr;
      model->map = &map;

      auto transform = entity->GetComponent<Transform>();
      transform->matrix = translate(transform->matrix, position);

      auto shape = CollisionShapes::NewBox(vec3(.5f, .5f, .5f));
      auto body = entity->AddComponent<RigidBody>(3.f, move(shape));

      body->ApplyCentralImpulse(direction * 30.f);
      body->SetRestitution(.2f);
      body->SetFriction(.8f);

      physics->SyncTransform(body);
      physics->AddRigidBody(body);
    }
  }

  void Initialize() {
    shaders = Loaders::Shaders("res/shaders");
    textures = Loaders::Textures("res/textures");

    map.diffuse = textures->Lookup("box_diffuse");
    map.specular = textures->Lookup("box_specular");
    map.shininess = 32.f;

    freecam = MakeShared<FreelookController>();
    freecam->SetPosition(vec3(0.f, 0.5f, 4.f));

    bus->Connect<KeyEvent>(freecam, &FreelookController::OnKeyEvent);
    bus->Connect<MouseMove>(freecam, &FreelookController::OnMouseMove);
    bus->Connect<KeyEvent>(this, &Game::OnKeyEvent);

    window->SetCursorMode(CursorMode::DISABLED);
    renderer->SetViewport(window->GetViewport());

    cubeBuffer =
        GLVertexArray::Create(sizeof(cubeVertices), cubeVertices, GLAttribute3f,
                              GLAttribute3f, GLAttribute2f);

    auto lightBuffer = Sphere::MakeSphere(.1f, 20, 10);
    sphereBuffer = Sphere::MakeSphere(.10795f, 100, 50);

    light = MakeShared<Model>(lightBuffer, shaders->Lookup("LightSource"));
    this->MakeGround();
  }

  void MakeGround() {
    Entity* entity = scene->CreateEntity();
    quadBuffer = Plane::MakePlane(50, 1.f);
    auto obj =
        entity->AddComponent<Model>(quadBuffer, shaders->Lookup("Lighting"));
    obj->texture = textures->Lookup("wall");
    auto shape =
        MakeUnique<btStaticPlaneShape>(btVector3(0, 1, 0), btScalar(0));
    auto ground = entity->AddComponent<RigidBody>(0.f, move(shape));
    ground->SetRollingFriction(.01f);
    ground->SetSpinningFriction(.01f);
    ground->SetFriction(.6f);
    ground->SetRestitution(1.f);
    physics->AddRigidBody(ground);
  }

  void Run() {
    mat4 projection = perspective(radians(65.f), 1024.f / 576.f, 0.2f, 3500.f);
    const double fixedTimeStep = 1.f / 60.f;

    float prevFrame = static_cast<float>(glfwGetTime());
    while (window->IsOpen()) {
      float currentFrame = static_cast<float>(glfwGetTime());
      float frameTime = currentFrame - prevFrame;
      int numSteps = static_cast<int>(frameTime / fixedTimeStep) + 1;
      prevFrame = currentFrame;

      physics->Update(frameTime, numSteps, fixedTimeStep);
      freecam->Update(frameTime);
      renderer->Clear();

      mat4 view = freecam->GetViewTransform();
      vec3 viewPosition = freecam->GetPosition();

      vec3 unit = vec3(2 * cos(currentFrame), 1.f, 2 * sin(currentFrame));
      light->matrix = translate(mat4(1.f), unit);
      vec4 lightPosition = light->matrix * vec4(0.f, 0.f, 0.f, 1.f);

      auto lighting = shaders->Lookup("Lighting");
      lighting->Enable();
      lighting->SetUniform("view", view);
      lighting->SetUniform("projection", projection);
      lighting->SetUniform("viewPosition", viewPosition);
      lighting->SetUniform("lightColor", vec3(1.0, 0.9803, 0.8039));
      lighting->SetUniform("lightPosition", vec3(lightPosition));

      auto bpMaterials = shaders->Lookup("bp-light-map");
      bpMaterials->Enable();
      bpMaterials->SetUniform("view", view);
      bpMaterials->SetUniform("projection", projection);
      bpMaterials->SetUniform("viewPosition", viewPosition);
      bpMaterials->SetUniform("light.ambient", vec3(0.2f, 0.2f, 0.2f));
      bpMaterials->SetUniform("light.diffuse", vec3(0.5f, 0.5f, 0.5f));
      bpMaterials->SetUniform("light.specular", vec3(1.f, 1.f, 1.f));
      bpMaterials->SetUniform("light.position", vec3(lightPosition));

      // TODO replace with cache friendly view iterator
      for (auto entity : scene->GetEntitiesWith<Model>()) {
        auto transform = entity->GetComponent<Transform>();
        auto object = entity->GetComponent<Model>();
        auto shader = object->program;
        mat3 objectNormalMatrix = mat3(transpose(inverse(transform->matrix)));
        shader->Enable();
        shader->SetUniform("model", transform->matrix);
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
  Shared<Atlas<GLTexture2D>> textures;
  Shared<Atlas<GLProgram>> shaders;
  Shared<Model> light;
  Shared<GLVertexArray> sphereBuffer;
  Shared<GLVertexArray> cubeBuffer;
  Shared<GLVertexArray> quadBuffer;
  Shared<PhysicsSystem> physics;

  LightMap map;
  Shared<Bus> bus;
  Shared<Scene> scene;
  Shared<GLWindow> window;
  Shared<GLRenderer> renderer;
  Shared<FreelookController> freecam;
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