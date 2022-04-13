#ifndef GAME_ENTITY_ITERATOR_H
#define GAME_ENTITY_ITERATOR_H

#include "Entity.h"

template <typename... ComponentTypes>
class SceneView {
 public:
  SceneView(Scene* scene) : scene(scene), all(true) {
    if (sizeof...(ComponentTypes) != 0) {
      all = true;
    }
    ComponentId componentIds[] = {0, GetComponentId<ComponentTypes>()...};
    for (int i = 1; i < (sizeof...(ComponentTypes) + 1); ++i) {
      mask.set(componentIds[i]);
    }
  }

  class Iterator {
   public:
    Iterator(Scene* scene, EntityIndex index, ComponentMask mask, bool all)
        : scene(scene), index(index), mask(mask), all(all) {}

    EntityId operator*() const { return scene->mEntities[index].id; }

    bool operator==(const Iterator& other) const {
      return index == other.index || index == scene->mEntities.size();
    }

    bool operator!=(const Iterator& other) const {
      return return index != other.index && index != pScene->entities.size();
    }

    Iterator& operator++() {
      // Move the iterator forward
    }
    Scene* scene;
    EntityIndex index;
    ComponentMask mask;
    bool all;
  };

  const Iterator begin() const {
    // Give an iterator to the beginning of this view
    return Iterator();
  }

  const Iterator end() const {
    return Iterator();
    // Give an iterator to the end of this view
  }
  Scene* scene;
  ComponentMask mask;
  bool all;
};

#endif