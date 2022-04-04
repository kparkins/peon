/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

//#include <vld.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "Peon.h"
#include "event/Event.h"
#include "event/EventDispatcher.h"
#include "event/WindowEvent.h"
#include "graphics/GLContext.h"
#include "graphics/GLProgram.h"
#include "graphics/GLVertexBuffer.h"
#include "graphics/GLWindow.h"
#include "log/StdoutStream.h"
#include "profile/BlockTimer.h"
#include "profile/RecordKeeper.h"

using std::cerr;
using std::cout;
using std::endl;
using std::unordered_map;

using namespace Peon;
using namespace glm;

static float triangleVertices[18] = {0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
                                     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

static float quadVertices[36] = {
    -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
    0.0f,  1.0f,  0.0f, 0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f,

    -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.5f, 0.5f,  0.0f,
    0.0f,  0.0f,  1.0f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f};

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f,
    0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    1.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f,
    1.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  1.0f};

namespace Peon {

class SceneNode {
 public:
  SceneNode(Shared<GLProgram> program, Shared<GLVertexBuffer> buffer)
      : mProgram(program), mBuffer(buffer) {}

  void Draw() {
    mProgram->Enable();
    mBuffer->Bind();
    mBuffer->Draw();
    mBuffer->Unbind();
    mProgram->Disable();
    for (auto c : mChildren) {
      c->Draw();
    }
  }

  void AddChild(Shared<SceneNode> child) { mChildren.push_back(child); }

 protected:
  Shared<GLVertexBuffer> mBuffer;
  Shared<GLProgram> mProgram;
  vector<Shared<SceneNode>> mChildren;
};

class GLRenderer {
 public:
  GLRenderer() {}

  ~GLRenderer() {}

  void SetTarget(const GLSurface& target) {
    glViewport(target.x, target.y, static_cast<GLsizei>(target.w),
               static_cast<GLsizei>(target.h));
    glEnable(GL_DEPTH_TEST);
  }

  void Render(Shared<SceneNode> scene) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->Draw();
  };
};

class Game : public EventListener<WindowEvent> {
 public:
  Game() {}

  ~Game() {}

  void Initialize() {
    PEON_INITIALIZE;

    gLogger.AddStream(MakeUnique<StdoutStream>());
    gLogger.SetLogLevel(LogLevel::TRACE);

    GLContextSettings ctxSettings;
    ctxSettings.forwardCompatible = false;
    ctxSettings.profile = PEON_OPENGL_CORE_PROFILE;
    ctxSettings.versionMajor = 4;
    ctxSettings.versionMinor = 0;

    GLVideoMode videoMode(640, 480);
    mWindow = MakeUnique<GLWindow>(videoMode, ctxSettings);

    mProgram = MakeShared<GLProgram>();
    mProgram->AddShader(GL_VERTEX_SHADER, "res/shaders/Default.vert");
    mProgram->AddShader(GL_FRAGMENT_SHADER, "res/shaders/Default.frag");
    mProgram->LinkProgram();

    mRenderer = MakeUnique<GLRenderer>();
    mRenderer->SetTarget(mWindow->GetSurface());

    Shared<GLVertexBuffer> buffer = GLVertexBuffer::Create(
        sizeof(cubeVertices), cubeVertices, GLAttribute3f(), GLAttribute3f());
    this->mScene = MakeShared<SceneNode>(mProgram, buffer);
  }

  void Run() {
    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -3.0f));

    glm::mat4 projection =
        glm::perspective(glm::radians(60.f), 640.f / 480.f, 0.1f, 100.f);
    while (mWindow->IsOpen()) {
      float time = static_cast<float>(glfwGetTime());
      model = glm::rotate(
          model, glm::radians(0.025f),
          glm::normalize(vec3(glm::sin(time), 0.f, glm::cos(-time))));
      mProgram->Enable();
      mProgram->SetUniform("model", model);
      mProgram->SetUniform("view", view);
      mProgram->SetUniform("projection", projection);
      mProgram->Disable();
      mRenderer->Render(mScene);
      mWindow->SwapBuffers();
    }
  }

 private:
  Shared<SceneNode> mScene;
  Shared<GLProgram> mProgram;
  Unique<GLWindow> mWindow;
  Unique<GLRenderer> mRenderer;
};
}  // namespace Peon

int main(int argc, char* argv[]) {
  Game g;
  g.Initialize();
  g.Run();

  return 0;
}
