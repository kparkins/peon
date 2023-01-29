#ifndef PEON_BP_TEXTURED_PASS_H
#define PEON_BP_TEXTURED_PASS_H

#include "Model.h"
#include "Renderable.h"
#include "entity/Scene.h"

namespace Peon {

struct BPTexturedPass : public Renderable {
  BPTexturedPass(GLProgram* shader) : shader(shader) {}

  void Render(GLRenderer* renderer, Scene* scene, Camera* camera) override {
    auto entities = scene->EntitiesWith<Light>();
    if (entities.begin() == entities.end()) {
      return;
    }
    auto lightEntity = *entities.begin();
    auto light = lightEntity->GetComponent<Light>();
    auto lightTransform = lightEntity->GetComponent<Transform>();

    shader->Enable();
    shader->SetUniform("view", camera->GetViewMatrix());
    shader->SetUniform("projection", camera->GetProjectionMatrix());
    shader->SetUniform("viewPosition", camera->GetPosition());
    shader->SetUniform("lightColor", light->diffuse);
    shader->SetUniform("lightPosition", lightTransform->GetWorldPosition());

    for (auto entity : scene->EntitiesWith<BPTextured>()) {
      auto bpt = entity->GetComponent<BPTextured>();
      auto transform = entity->GetComponent<Transform>();
      auto object = entity->GetComponent<Model>();
      mat3 objectNormalMatrix = mat3(transpose(inverse(transform->matrix)));
      bpt->texture->Bind();
      shader->SetUniform("model", transform->matrix);
      shader->SetUniform("normalMatrix", objectNormalMatrix);
      object->buffer->Bind();
      object->buffer->Draw();
    }
  }

  GLProgram* shader;
};

}  // namespace Peon

#endif