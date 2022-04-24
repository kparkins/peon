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
#include "components/Components.h"
#include "event/Bus.h"
#include "event/MouseEvent.h"
#include "graphics/Plane.h"
#include "graphics/Sphere.h"
#include "graphics/opengl/GLContext.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLRenderer.h"
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

static float cubeVertexNormalTexture[] = {
    // Back face
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.f,
    0.f, -1.f, 1.0f, 1.0f, 0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 1.0f, 0.0f, 0.5f,
    0.5f, -0.5f, 0.f, 0.f, -1.f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.f, 0.f,
    -1.f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.f, 0.f, -1.f, 0.0f, 1.0f,
    // Front face
    -0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.f, 0.f,
    1.f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f, 1.0f, 1.0f, 0.5f, 0.5f,
    0.5f, 0.f, 0.f, 1.f, 1.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f, 0.0f,
    1.0f, -0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 0.0f, 0.0f,
    // Left face
    -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.f,
    0.f, 0.f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, -1.f,
    0.f, 0.f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.f, 0.f,
    0.f, 0.0f, 1.0f, 0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 1.0f, 1.0f, 0.5f, -0.5f,
    -0.5f, 1.f, 0.f, 0.f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.0f,
    0.0f, 0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 0.0f, 0.0f,
    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 0.f,
    -1.f, 0.f, 1.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f, 1.0f, 0.0f, 0.5f,
    -0.5f, 0.5f, 0.f, -1.f, 0.f, 1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f,
    0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.0f, 1.0f,
    // Top face
    -0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.f, 1.f,
    0.f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f, 0.5f, 0.5f,
    0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 0.0f,
    1.0f, -0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f};

static float quadVertices[] = {
    -0.5f, 0,   -0.5f, 0.f, 1.f,  0.f, 0.f,   1.f, -0.5f, 0,   0.5f, 0.f,
    1.f,   0.f, 0.f,   0.f, 0.5f, 0,   -0.5f, 0.f, 1.f,   0.f, 1.f,  1.f,

    0.5f,  0,   -0.5f, 0.f, 1.f,  0.f, 1.f,   1.f, -0.5f, 0,   0.5f, 0.f,
    1.f,   0.f, 0.f,   0.f, 0.5f, 0,   0.5f,  0.f, 1.f,   0.f, 1.f,  0.f,
};

namespace Peon {

typedef struct Model {
  Model(Shared<GLVertexArray> buffer, GLProgram* program)
      : buffer(buffer), program(program), texture(nullptr) {}

  void Draw() {
    if (texture) {
      texture->Bind();
    }
    ////   if (map) {
    //     map->diffuse->SetTextureUnit(GLTextureUnit::TEXTURE0);
    //     map->diffuse->Bind();
    //     map->specular->SetTextureUnit(GLTextureUnit::TEXTURE1);
    //      map->specular->Bind();
    // program->Enable();
    //  program->SetUniform("objectMap.diffuse", 0);
    // program->SetUniform("objectMap.specular", 1);
    //   program->SetUniform("objectMap.shininess", map->shininess);
    //    }
    buffer->Bind();
    buffer->Draw();
    buffer->Unbind();
    // program->Disable();
  }

  GLProgram* program;
  GLTexture2D* texture;
  Shared<GLVertexArray> buffer;
  mat4 matrix;
} Model;

class RenderPass {
 public:
  virtual void Render(GLRenderer* renderer, Scene* scene, Camera* camera) = 0;
};

struct PhongLightMapPass : public RenderPass {
  PhongLightMapPass(GLProgram* shader) : shader(shader) {}

  void Render(GLRenderer* renderer, Scene* scene, Camera* camera) override {
    auto lights = scene->GetEntitiesWith<Light>();
    if (lights.size() < 1) {
      return;
    }
    auto light = lights[0]->GetComponent<Light>();
    shader->Enable();
    shader->SetUniform("object.diffuse", 0);
    shader->SetUniform("object.specular", 1);
    shader->SetUniform("view", camera->GetViewMatrix());
    shader->SetUniform("viewPosition", camera->GetPosition());
    shader->SetUniform("projection", camera->GetProjectionMatrix());
    shader->SetUniform("light.ambient", light->ambient);
    shader->SetUniform("light.diffuse", light->diffuse);
    shader->SetUniform("light.specular", light->specular);
    shader->SetUniform("light.position", light->position);
    for (auto& entity : scene->GetEntitiesWith<BPLightMap>()) {
      auto map = entity->GetComponent<BPLightMap>();
      auto model = entity->GetComponent<Model>();
      auto transform = entity->GetComponent<Transform>();
      mat3 objectNormalMatrix = mat3(transpose(inverse(transform->matrix)));

      shader->SetUniform("object.shininess", map->shininess);
      shader->SetUniform("model", transform->matrix);
      shader->SetUniform("normalMatrix", objectNormalMatrix);

      map->diffuse->SetTextureUnit(GLTextureUnit::TEXTURE0);
      map->diffuse->Bind();
      map->specular->SetTextureUnit(GLTextureUnit::TEXTURE1);
      map->specular->Bind();

      model->buffer->Bind();
      model->buffer->Draw();
    }
  }

  GLProgram* shader;
};

struct BPTexturedPass : public RenderPass {
  BPTexturedPass(GLProgram* shader) : shader(shader) {}

  void Render(GLRenderer* renderer, Scene* scene, Camera* camera) override {
    auto entities = scene->GetEntitiesWith<Light>();
    if (entities.size() < 1) {
      return;
    }
    auto light = entities[0]->GetComponent<Light>();

    shader->Enable();
    shader->SetUniform("view", camera->GetViewMatrix());
    shader->SetUniform("projection", camera->GetProjectionMatrix());
    shader->SetUniform("viewPosition", camera->GetPosition());
    shader->SetUniform("lightColor", light->diffuse);
    shader->SetUniform("lightPosition", light->position);

    for (auto entity : scene->GetEntitiesWith<BPTextured>()) {
      auto bpt = entity->GetComponent<BPTextured>();
      auto transform = entity->GetComponent<Transform>();
      auto object = entity->GetComponent<Model>();
      mat3 objectNormalMatrix = mat3(transpose(inverse(transform->matrix)));
      bpt->texture->Bind();
      shader->SetUniform("model", transform->matrix);
      shader->SetUniform("normalMatrix", objectNormalMatrix);
      object->Draw();
    }
  }

  GLProgram* shader;
};

class Game {
 public:
  Game(Shared<GLWindow> window, Shared<Bus> bus) : window(window), bus(bus) {
    scene = MakeShared<Scene>();
    physics = MakeShared<PhysicsSystem>(scene, bus);
    renderer = MakeShared<GLRenderer>();
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

      auto bptexture = entity->AddComponent<BPTextured>();
      bptexture->texture = textures->Lookup("ball");

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

    } else if (event.key == Key::LEFT_CONTROL) {
      Entity* entity = scene->CreateEntity();

      auto shading = entity->AddComponent<BPLightMap>();
      shading->diffuse = textures->Lookup("box_diffuse");
      shading->specular = textures->Lookup("box_specular");
      shading->shininess = 32.f;

      auto model = entity->AddComponent<Model>(cubeBuffer,
                                               shaders->Lookup("bp-light-map"));

      auto transform = entity->GetComponent<Transform>();
      transform->matrix = translate(transform->matrix, position);

      auto shape = CollisionShapes::NewBox(vec3(.52f, .52f, .52f));
      shape->setMargin(btScalar(.1f));
      auto body = entity->AddComponent<RigidBody>(10.f, move(shape));

      body->ApplyCentralImpulse(direction * 60.f);
      body->SetRestitution(.2f);
      body->SetFriction(.8f);

      physics->SyncTransform(body);
      physics->AddRigidBody(body);
    }
  }

  void Initialize() {
    shaders = Loaders::Shaders("res/shaders");
    textures = Loaders::Textures("res/textures");

    freecam = MakeShared<FreelookController>();
    freecam->SetPosition(vec3(0.f, 0.5f, 4.f));
    freecam->SetProjectionMatrix(
        perspective(radians(65.f), 1024.f / 576.f, 0.2f, 3500.f));

    bus->Connect<KeyEvent>(freecam, &FreelookController::OnKeyEvent);
    bus->Connect<MouseMove>(freecam, &FreelookController::OnMouseMove);
    bus->Connect<KeyEvent>(this, &Game::OnKeyEvent);

    window->SetCursorMode(CursorMode::DISABLED);

    renderer->SetViewport(window->GetViewport());
    renderer->Enable(GLOption::MSAA);

    renderer->Enable(GLOption::DEPTH_TEST);
    renderer->SetDepthFunc(GLDepthFunc::LESS);

    renderer->Enable(GLOption::CULL_FACE);
    renderer->SetCullFace(GLCullFace::BACK, GLFrontFace::CCW);
    renderer->SetClearColor(vec4(0.1f, 0.1f, 0.1f, 0.f));

    cubeBuffer = GLVertexArray::Create(sizeof(cubeVertexNormalTexture),
                                       cubeVertexNormalTexture, GLAttribute3f,
                                       GLAttribute3f, GLAttribute2f);
    sphereBuffer = Sphere::MakeSphere(.10795f, 100, 50);
    auto lightBuffer = Sphere::MakeSphere(.1f, 20, 10);

    light = MakeShared<Model>(lightBuffer, shaders->Lookup("LightSource"));

    auto lightEntity = scene->CreateEntity();

    auto lightComponent = lightEntity->AddComponent<Light>();
    lightComponent->ambient = vec3(0.2f, 0.2f, 0.2f);
    lightComponent->diffuse = vec3(0.5f, 0.5f, 0.5f);
    lightComponent->specular = vec3(1.f, 1.f, 1.f);

    lightEntity->AddComponent<Model>(lightBuffer,
                                     shaders->Lookup("LightSource"));

    this->MakeGround();
  }

  void MakeGround() {
    Entity* entity = scene->CreateEntity();
    quadBuffer = Plane::MakePlane(50, 1.f);
    auto obj =
        entity->AddComponent<Model>(quadBuffer, shaders->Lookup("Lighting"));
    auto bptexture = entity->AddComponent<BPTextured>();
    bptexture->texture = textures->Lookup("wall");
    auto shape =
        MakeUnique<btStaticPlaneShape>(btVector3(0, 1, 0), btScalar(0));
    auto ground = entity->AddComponent<RigidBody>(0.f, move(shape));
    ground->SetRollingFriction(.01f);
    ground->SetSpinningFriction(.01f);
    ground->SetFriction(.6f);
    ground->SetRestitution(1.f);
    physics->AddRigidBody(ground);
  }

  float UpdateSystems(float previousFrame) {
    const double fixedTimeStep = 1.f / 60.f;
    float currentFrame = static_cast<float>(glfwGetTime());
    float frameTime = currentFrame - previousFrame;
    int numSteps = static_cast<int>(frameTime / fixedTimeStep) + 1;
    physics->Update(frameTime, numSteps, fixedTimeStep);
    freecam->Update(frameTime);

    vec3 unit = vec3(2 * cos(currentFrame), 1.f, 2 * sin(currentFrame));
    light->matrix = translate(mat4(1.f), unit);
    return currentFrame;
  }

  void Run() {
    BPTexturedPass bpTexturedPass(shaders->Lookup("bp-textured"));
    PhongLightMapPass lightMapPass(shaders->Lookup("bp-light-map"));

    float lastFrame = static_cast<float>(glfwGetTime());
    while (window->IsOpen()) {
      lastFrame = this->UpdateSystems(lastFrame);

      renderer->Clear();

      lightMapPass.Render(renderer.get(), scene.get(), freecam.get());
      bpTexturedPass.Render(renderer.get(), scene.get(), freecam.get());

      // todo make this a different pass, the position is not updating in other
      // shaders atm
      auto lightProgram = light->program;

      lightProgram->Enable();
      lightProgram->SetUniform("modelView",
                               freecam->GetViewMatrix() * light->matrix);
      lightProgram->SetUniform("projection", freecam->GetProjectionMatrix());
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
