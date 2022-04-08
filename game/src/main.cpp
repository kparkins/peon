/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

//#include <vld.h>

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

#include "Peon.h"
#include "event/Event.h"
#include "event/EventDispatcher.h"
#include "event/KeyListener.h"
#include "event/MouseEvent.h"
#include "event/WindowListener.h"
#include "graphics/FreelookCamera.h"
#include "graphics/opengl/GLContext.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLTexture2D.h"
#include "graphics/opengl/GLVertexBuffer.h"
#include "graphics/opengl/GLWindow.h"
#include "input/Key.h"
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

static float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

namespace Peon {

class SceneNode {
 public:
  SceneNode(Shared<GLProgram> program, Shared<GLVertexBuffer> buffer)
      : mProgram(program), mBuffer(buffer) {}

  void Draw() {
    mBuffer->Bind();
    mBuffer->Draw();
    mBuffer->Unbind();
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
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->Draw();
  };
};

class Game {
 public:
  Game(Shared<GLWindow> window) : mWindow(window) {}

  ~Game() {}

  void Initialize() {
    GLShader fragment(ShaderType::FRAGMENT);
    fragment.Load("res/shaders/2Texture.frag");

    mWindow->SetCursorMode(CursorMode::DISABLED);

    GLShader vertex(ShaderType::VERTEX);
    vertex.Load("res/shaders/2Texture.vert");

    mProgram = MakeShared<GLProgram>(vertex, fragment);

    mTexture = MakeShared<GLTexture2D>("res/textures/paper.jpg");
    mTexture->SetTextureUnit(GLTextureUnit::TEXTURE0);

    mOtherTexture = MakeShared<GLTexture2D>("res/textures/kourosh_large.jpg");
    mOtherTexture->SetTextureUnit(GLTextureUnit::TEXTURE1);

    mRenderer->SetTarget(mWindow->GetSurface());

    Shared<GLVertexBuffer> buffer = GLVertexBuffer::Create(
        sizeof(cubeVertices), cubeVertices, GLAttribute3f, GLAttribute2f);
    this->mScene = MakeShared<SceneNode>(mProgram, buffer);
  }

  void Run() {
    mat4 model = mat4(1.f);
    float prevFrame = glfwGetTime();

    mat4 projection = perspective(radians(60.f), 640.f / 480.f, 0.1f, 100.f);
    while (mWindow->IsOpen()) {
      float currentFrame = static_cast<float>(glfwGetTime());
      float dt = currentFrame - prevFrame;
      prevFrame = currentFrame;
      mFreelook.Update(dt);
      model = rotate(model, radians(0.002f), vec3(0.f, 0.f, 1.f));
      mat4 trans = translate(model, vec3(-1.f, 0.f, 0.f));
      mTexture->Bind();
      mOtherTexture->Bind();
      mProgram->Enable();
      mProgram->SetUniform("model", trans);
      mProgram->SetUniform("view", mFreelook.GetViewTransform());
      mProgram->SetUniform("projection", projection);
      mProgram->SetUniform("tex_sampler1", 0);
      mProgram->SetUniform("tex_sampler2", 1);
      mRenderer->Render(mScene);
      mProgram->Disable();
      mTexture->Unbind();
      mOtherTexture->Unbind();
      mWindow->SwapBuffers();
    }
  }

 private:
  FreeLookCamera mFreelook;
  Shared<GLTexture2D> mTexture;
  Shared<GLTexture2D> mOtherTexture;
  Shared<SceneNode> mScene;
  Shared<GLProgram> mProgram;
  Shared<GLWindow> mWindow;
  Shared<GLRenderer> mRenderer;
};
}  // namespace Peon

int main(int argc, char* argv[]) {
  PEON_INITIALIZE;

  gLogger.AddStream(MakeUnique<StdoutStream>());
  gLogger.SetLogLevel(LogLevel::TRACE);

  GLContextOpts ctxOpts;
  ctxOpts.forwardCompatible = false;
  ctxOpts.profile = PEON_OPENGL_CORE_PROFILE;
  ctxOpts.versionMajor = 4;
  ctxOpts.versionMinor = 0;

  GLVideoMode videoMode(640, 480);

  auto window = MakeShared<GLWindow>(ctxOpts, videoMode);

  Game g(window);

  try {
    g.Initialize();
    g.Run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}