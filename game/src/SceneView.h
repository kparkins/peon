#ifndef GAME_ENTITY_ITERATOR_H
#define GAME_ENTITY_ITERATOR_H

#include "Component.h"
#include "Entity.h"
#include "Scene.h"

template <typename... ComponentTypes>
class SceneView {
 public:
  SceneView(Scene* scene) : scene(scene), all(true) {
    if (sizeof...(ComponentTypes) != 0) {
      all = false;
    }
    ComponentId componentIds[] = {0, GetComponentId<ComponentTypes>()...};
    for (int i = 1; i < (sizeof...(ComponentTypes) + 1); ++i) {
      pools.push_back(scene->mComponents[componentIds[i]]));
    }
  }

  struct Iterator {
    Iterator(Scene* scene, ComponentMask mask, bool all)
        : scene(scene), index(index), mask(mask), all(all) {}

    EntityId operator*() const { return scene->mEntities[index]; }

    bool operator==(const Iterator& other) const {
      return index == other.index;
    }

    bool operator!=(const Iterator& other) const {
      return index != other.index;
    }

    Iterator& operator++() {
      while (index < scene->mEntities.size()) {
      }
    }

    Iterator operator++(int) { return nullptr; }

   private:
    Scene* scene;
    size_t poolIndex;
    size_t entityIndex;
    ComponentMask mask;
    bool all;
  };

  const Iterator begin() const { return Iterator(scene, 0, mask, all); }

  const Iterator end() const { return Iterator(); }
  Scene* scene;
  vector<Pool*> pools;
  bool all;
};

#endif