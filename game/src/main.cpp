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

#include "entity/ComponentTypes.h"
#include "entity/EntityView.h"
#include "graphics/Loaders.h"
#include "Peon.h"
#include "entity/Scene.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
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
#include "graphics/Renderable.h"
#include "graphics/BPLightMapPass.h"
#include "graphics/BPTexturedPass.h"
#include "graphics/StaticColorPass.h"

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

// TODO
// X1. refactor the drawing of light to a render pass
// X2. fix this Model struct after 1.
// 3. Write scene view iterator.
//     - should be able to select multiple components at once
//     - this will allow filter like behavior and destructuring in the loop
//       definition
// 4. Add support for multiple lights
// 5. Try to generalize render passes, in particular uniform linkage.



class RenderSequence : public Renderable {
public:

    void AddPass(Shared<Renderable> pass) {
        mPasses.push_back(pass);
    }

    void Render(GLRenderer* renderer, Scene* scene, Camera* camera) override {
        renderer->Clear();
        for (auto& pass : mPasses) {
            pass->Render(renderer, scene, camera);
        }
    }

    vector<Shared<Renderable>> mPasses;
};


class Game {
 public:
  Game(Shared<GLWindow> window, Shared<Bus> bus) : window(window), bus(bus) {
    scene = MakeShared<Scene>();
    physics = MakeShared<PhysicsSystem>(scene, bus);
    renderer = MakeShared<GLRenderer>();
  }

  ~Game() {}

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

    auto bpTexturedPass = MakeShared<BPTexturedPass>(shaders->Lookup("bp-textured"));
    auto bpLightPass = MakeShared<BPLightMapPass>(shaders->Lookup("bp-light-map"));
    auto staticColorPass = MakeShared<StaticColorPass>(shaders->Lookup("LightSource"));

    renderSequence = MakeShared<RenderSequence>();
    renderSequence->AddPass(bpTexturedPass);
    renderSequence->AddPass(bpLightPass);
    renderSequence->AddPass(staticColorPass);

    this->MakeLight();
    this->MakeGround();
  }

  virtual void OnKeyEvent(const KeyEvent& event) {
    if (event.action != KeyAction::PRESS) {
      return;
    }
    if (event.key == Key::SPACE) {
      this->SpawnBall();
    } else if (event.key == Key::LEFT_CONTROL) {
      this->SpawnBox();
    }
  }

  void SpawnBall() {
    vec3 direction = freecam->GetLookDirection();
    vec3 position = freecam->GetPosition();
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

    physics->SyncTransform(entity);
    physics->AddRigidBody(body);

    auto model = entity->AddComponent<Model>(sphereBuffer);
  }

  void SpawnBox() {
    vec3 direction = freecam->GetLookDirection();
    vec3 position = freecam->GetPosition();
    Entity* entity = scene->CreateEntity();

    auto shading = entity->AddComponent<BPLightMap>();
    shading->diffuse = textures->Lookup("box_diffuse");
    shading->specular = textures->Lookup("box_specular");
    shading->shininess = 32.f;

    auto model = entity->AddComponent<Model>(cubeBuffer);

    auto transform = entity->GetComponent<Transform>();
    transform->matrix = translate(transform->matrix, position);

    auto shape = CollisionShapes::NewBox(vec3(.52f, .52f, .52f));
    shape->setMargin(btScalar(.1f));
    auto body = entity->AddComponent<RigidBody>(10.f, move(shape));

    body->ApplyCentralImpulse(direction * 60.f);
    body->SetRestitution(.2f);
    body->SetFriction(.8f);

    physics->SyncTransform(entity);
    physics->AddRigidBody(body);
  }

  void MakeLight() {
    auto lightBuffer = Sphere::MakeSphere(.1f, 20, 10);

    light = scene->CreateEntity();
    auto lightComponent = light->AddComponent<Light>();
    lightComponent->ambient = vec3(0.2f, 0.2f, 0.2f);
    lightComponent->diffuse = vec3(0.5f, 0.5f, 0.5f);
    lightComponent->specular = vec3(1.f, 1.f, 1.f);

    light->AddComponent<Model>(lightBuffer);
    auto color = light->AddComponent<StaticColor>();
    color->color = vec3(1.0, 0.9803921568627451, 0.8039215686274511);
  }

  void MakeGround() {
    Entity* entity = scene->CreateEntity();
    quadBuffer = Plane::MakePlane(50, 1.f);
    auto obj = entity->AddComponent<Model>(quadBuffer);
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
    auto lightTransform = light->GetComponent<Transform>();
    lightTransform->matrix = translate(mat4(1.f), unit);
    return currentFrame;
  }

  void Run() {
    float lastFrame = static_cast<float>(glfwGetTime());
    while (window->IsOpen()) {

      lastFrame = this->UpdateSystems(lastFrame);
      renderSequence->Render(renderer.get(), scene.get(), freecam.get());

      window->SwapBuffers();
    }
  }

 private:
  Shared<Atlas<GLTexture2D>> textures;
  Shared<Atlas<GLProgram>> shaders;
  Shared<GLVertexArray> sphereBuffer;
  Shared<GLVertexArray> cubeBuffer;
  Shared<GLVertexArray> quadBuffer;
  Shared<PhysicsSystem> physics;
  Shared<RenderSequence> renderSequence;

  Entity* light;
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
  ctxOpts.forwardCompatible = true;
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
