#ifndef PEON_BP_LIGHT_MAP_PASS_H
#define PEON_BP_LIGHT_MAP_PASS_H

#include "Camera.h"
#include "Model.h"
#include "Renderable.h"
#include "entity/Scene.h"
#include "opengl/GLProgram.h"
#include "opengl/GLShader.h"

namespace Peon {

struct BPLightMapPass : public Renderable {
  BPLightMapPass(GLProgram* shader) : shader(shader) {}

  void Render(GLRenderer* renderer, Scene* scene, Camera* camera) override {
    auto lights = scene->EntitiesWith<Light>();
    if (lights.begin() == lights.end()) {
      return;
    }
    // For now we know there is only one light.
    auto lightEntity = *lights.begin();
    auto light = lightEntity->GetComponent<Light>();
    auto lightTransform = lightEntity->GetComponent<Transform>();

    shader->Enable();
    shader->SetUniform("object.diffuse", 0);
    shader->SetUniform("object.specular", 1);
    shader->SetUniform("view", camera->GetViewMatrix());
    shader->SetUniform("viewPosition", camera->GetPosition());
    shader->SetUniform("projection", camera->GetProjectionMatrix());
    shader->SetUniform("light.ambient", light->ambient);
    shader->SetUniform("light.diffuse", light->diffuse);
    shader->SetUniform("light.specular", light->specular);
    shader->SetUniform("light.position", lightTransform->GetWorldPosition());
    for (auto entity : scene->EntitiesWith<BPLightMap>()) {
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

}  // namespace Peon

#endif  // !GAME_BP_LIGHT_MAP_PASS_H
