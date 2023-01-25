#ifndef GAME_ENTITY_VIEW_H
#define GAME_ENTITY_VIEW_H

#include <vector>

#include "Component.h"
#include "EntityIterator.h"

class Entity;

using std::vector;

template <typename... IncludedTypes>
class EntityView {
 public:
  EntityView(vector<Entity*> entities);

  EntityIterator begin();
  EntityIterator end();

  const EntityIterator begin() const;
  const EntityIterator end() const;

 private:
  vector<Entity*> entities;
  ComponentMask included;
};

template <typename... IncludedTypes>
EntityView<IncludedTypes...>::EntityView(vector<Entity*> entities)
    : entities(entities) {
  ComponentId componentIds[] = {0, Component<IncludedTypes>::Id()...};
  size_t size = sizeof(componentIds) / sizeof(componentIds[0]);
  for (size_t i = 1; i < size; ++i) {
    included.set(componentIds[i]);
  }
}
template <typename... IncludedTypes>
EntityIterator EntityView<IncludedTypes...>::begin() {
  return EntityIterator(0, entities, included);
}

template <typename... IncludedTypes>
EntityIterator EntityView<IncludedTypes...>::end() {
  return EntityIterator(entities.size(), entities, included);
}

template <typename... IncludedTypes>
const EntityIterator EntityView<IncludedTypes...>::begin() const {
  return EntityIterator(0, entities, included);
}

template <typename... IncludedTypes>
const EntityIterator EntityView<IncludedTypes...>::end() const {
  return EntityIterator(entities.size(), entities, included);
}

#endif