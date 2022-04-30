#include "EntityIterator.h"
#include "Scene.h"

EntityIterator::EntityIterator(size_t index, vector<Entity*> entities,
                               ComponentMask included)
    : index(index), entities(entities), included(included) {
  if (entities.size() && !entities[0]->HasComponents(included)) {
    this->operator++();
  }
}

Entity* EntityIterator::operator*() { return entities[index]; }

const Entity* EntityIterator::operator*() const { return entities[index]; }

Entity* EntityIterator::operator->() { return entities[index]; }

const Entity* EntityIterator::operator->() const { return entities[index]; }

EntityIterator& EntityIterator::operator++() {
  size_t cursor = index + 1;
  while (cursor < entities.size() &&
         !entities[cursor]->HasComponents(included)) {
    ++cursor;
  }
  index = std::min(entities.size(), cursor);
  return *this;
}

EntityIterator& EntityIterator::operator--() {
  size_t cursor = index - 1;
  while (cursor >= 0 && !entities[cursor]->HasComponents(included)) {
    --cursor;
  }
  index = std::max(static_cast<size_t>(0), cursor);
  return *this;
}

EntityIterator EntityIterator::operator++(int) {
  EntityIterator r(*this);
  this->operator++();
  return r;
}

EntityIterator EntityIterator::operator--(int) {
  EntityIterator r(*this);
  this->operator--();
  return r;
}

EntityIterator& EntityIterator::operator+=(int n) {
  size_t count = 0;
  while (count < n && index != entities.size()) {
    this->operator++();
  }
  return *this;
}

EntityIterator& EntityIterator::operator-=(int n) {
  size_t count = 0;
  while (count < n && index >= 0) {
    this->operator--();
  }
  return *this;
}

EntityIterator EntityIterator::operator+(int n) const {
  EntityIterator r(*this);
  return r += n;
}

EntityIterator EntityIterator::operator-(int n) const {
  EntityIterator r(*this);
  return r -= n;
}

size_t EntityIterator::operator-(EntityIterator const& r) const {
  return index - r.index;
}

bool EntityIterator::operator<(EntityIterator const& r) const {
  return index < r.index;
}

bool EntityIterator::operator<=(EntityIterator const& r) const {
  return index <= r.index;
}

bool EntityIterator::operator>(EntityIterator const& r) const {
  return index > r.index;
}

bool EntityIterator::operator>=(EntityIterator const& r) const {
  return index >= r.index;
}

bool EntityIterator::operator!=(const EntityIterator& r) const {
  return index != r.index;
}

bool EntityIterator::operator==(const EntityIterator& r) const {
  return index == r.index;
}