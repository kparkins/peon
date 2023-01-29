#include "EntityIterator.h"
#include "Scene.h"

Peon::EntityIterator::EntityIterator(size_t index,
                                     vector<Entity*> entities,
                                     Peon::ComponentMask included)
    : index(index), entities(entities), included(included) {
  if (entities.size() && !entities[0]->HasComponents(included)) {
    this->operator++();
  }
}

Peon::Entity* Peon::EntityIterator::operator*() {
  return entities[index];
}

const Peon::Entity* Peon::EntityIterator::operator*() const {
  return entities[index];
}

Peon::Entity* Peon::EntityIterator::operator->() {
  return entities[index];
}

const Peon::Entity* Peon::EntityIterator::operator->() const {
  return entities[index];
}

Peon::EntityIterator& Peon::EntityIterator::operator++() {
  size_t cursor = index + 1;
  while (cursor < entities.size() &&
         !entities[cursor]->HasComponents(included)) {
    ++cursor;
  }
  index = std::min(entities.size(), cursor);
  return *this;
}

Peon::EntityIterator& Peon::EntityIterator::operator--() {
  size_t cursor = index - 1;
  while (cursor >= 0 && !entities[cursor]->HasComponents(included)) {
    --cursor;
  }
  index = std::max(static_cast<size_t>(0), cursor);
  return *this;
}

Peon::EntityIterator Peon::EntityIterator::operator++(int) {
  EntityIterator r(*this);
  this->operator++();
  return r;
}

Peon::EntityIterator Peon::EntityIterator::operator--(int) {
  EntityIterator r(*this);
  this->operator--();
  return r;
}

Peon::EntityIterator& Peon::EntityIterator::operator+=(int n) {
  size_t count = 0;
  while (count < n && index != entities.size()) {
    this->operator++();
  }
  return *this;
}

Peon::EntityIterator& Peon::EntityIterator::operator-=(int n) {
  size_t count = 0;
  while (count < n && index >= 0) {
    this->operator--();
  }
  return *this;
}

Peon::EntityIterator Peon::EntityIterator::operator+(int n) const {
  EntityIterator r(*this);
  return r += n;
}

Peon::EntityIterator Peon::EntityIterator::operator-(int n) const {
  EntityIterator r(*this);
  return r -= n;
}

size_t Peon::EntityIterator::operator-(EntityIterator const& r) const {
  return index - r.index;
}

bool Peon::EntityIterator::operator<(EntityIterator const& r) const {
  return index < r.index;
}

bool Peon::EntityIterator::operator<=(EntityIterator const& r) const {
  return index <= r.index;
}

bool Peon::EntityIterator::operator>(EntityIterator const& r) const {
  return index > r.index;
}

bool Peon::EntityIterator::operator>=(EntityIterator const& r) const {
  return index >= r.index;
}

bool Peon::EntityIterator::operator!=(const EntityIterator& r) const {
  return index != r.index;
}

bool Peon::EntityIterator::operator==(const EntityIterator& r) const {
  return index == r.index;
}