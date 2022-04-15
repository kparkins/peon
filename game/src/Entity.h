#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <cstdint>

#include "Component.h"
#include "Pool.h"
#include "common/Uncopyable.h"

using std::numeric_limits;

typedef uint64_t EntityId;
typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
const EntityIndex INVALID_ENTITY_INDEX = EntityIndex(-1);

class Scene;

class Entity : public Peon::Uncopyable {
 public:
  inline bool IsValid() const;

  inline EntityId GetId() const;
  inline EntityIndex GetIndex() const;
  inline EntityVersion GetVersion() const;

  template <typename T, typename... Args>
  inline Component<T> AddComponent();

  template <typename T>
  inline Component<T> GetComponent();

  template <typename T>
  inline void RemoveComponent();

  template <typename T>
  inline bool HasComponent() const;

  template <typename... Components>
  inline bool HasComponents() const;
  inline bool HasComponents(ComponentMask mask) const;

  inline ComponentMask GetComponents() const;

 protected:
  explicit Entity();
  explicit Entity(EntityIndex index, EntityVersion version);
  virtual ~Entity();

  void SetId(EntityIndex index, EntityVersion version);

  inline void Add(ComponentId id);
  inline void Remove(ComponentId id);

  friend class Scene;

  ComponentMask mComponents;
  Scene* mScene;
  EntityId mId;
};

inline EntityVersion Entity::GetVersion() const {
  return static_cast<EntityVersion>(mId & numeric_limits<uint32_t>::max());
}

inline EntityIndex Entity::GetIndex() const { return mId >> 32; }
inline EntityId Entity::GetId() const { return mId; }
inline bool Entity::IsValid() const {
  return (mId >> 32) != INVALID_ENTITY_INDEX;
}

template <typename T, typename... Args>
inline Component<T> Entity::AddComponent() {
  if (!this->IsValid()) {
    return Component<T>(nullptr, nullptr);
  }
  return mScene->AddComponent<T>(this, Args...);
}

template <typename T>
inline Component<T> Entity::GetComponent() {
  if (!this->IsValid()) {
    return Component(nullptr, nullptr);
  }
  return Component<T>(mScene, this);
}

template <typename T>
inline void Entity::RemoveComponent() {
  if (this->IsValid() && mScene->GetComponent<T>(this)) {
    mScene->RemoveComponent(this);
  }
}

template <typename T>
inline bool Entity::HasComponent() const {
  ComponentId id = Component<T>::Id();
  return this->mComponents.test(id);
}

template <typename... Components>
inline bool Entity::HasComponents() const {
  ComponentMask mask;
  ComponentId componentIds[] = {0, Component<ComponentTypes>::Id()...};
  for (int i = 1; i < (sizeof...(ComponentTypes) + 1); ++i) {
    mask.set(componentIds[i]);
  }
  return mask == this->mComponents;
}

inline bool Entity::HasComponents(ComponentMask mask) const {
  return mask == this->mComponents;
}

inline ComponentMask Entity::GetComponents() const { return this->mComponents; }

inline void Entity::Add(ComponentId id) { this->mComponents.set(id); }
inline void Entity::Remove(ComponentId id) { this->mComponents.reset(id); }

#endif