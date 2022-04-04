#ifndef PEON_TYPE_ALIASES_H
#define PEON_TYPE_ALIASES_H

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Peon {

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

template <typename T>
inline Shared<T> MakeShared(Unique<T> object) {
  return Shared<T>(std::move(object));
}

template <typename T, typename... Args>
inline Shared<T> MakeShared(Args&&... args) {
  return Shared<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
inline Unique<T> MakeUnique(Args&&... args) {
  return Unique<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
inline Weak<T> MakeWeak(Args&&... args) {
  return std::weak_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace Peon

#endif