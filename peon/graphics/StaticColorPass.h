#ifndef PEON_STATIC_COLOR_PASS_H
#define PEON_STATIC_COLOR_PASS_H

#include "Camera.h"
#include "Model.h"
#include "Renderable.h"
#include "entity/Scene.h"

namespace Peon {

struct StaticColorPass : public Renderable {
  StaticColorPass(GLProgram* program) : shader(program) {}

  void Render(GLRenderer* renderer, Scene* scene, Camera* camera) {
    mat4 view = camera->GetViewMatrix();
    shader->Enable();
    shader->SetUniform("projection", camera->GetProjectionMatrix());
    for (auto entity : scene->EntitiesWith<StaticColor>()) {
      auto transform = entity->GetComponent<Transform>();
      auto model = entity->GetComponent<Model>();
      auto color = entity->GetComponent<StaticColor>();
      shader->SetUniform("modelView", view * transform->matrix);
      shader->SetUniform("color", color->color);
      model->buffer->Bind();
      model->buffer->Draw();
    }
  }

  GLProgram* shader;
};

}  // namespace Peon

#endif