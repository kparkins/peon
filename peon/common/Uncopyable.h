#ifndef PEON_UNCOPYABLE_H
#define PEON_UNCOPYABLE_H

namespace Peon {
class Uncopyable {
 protected:
  Uncopyable() = default;
  virtual ~Uncopyable() = default;

  Uncopyable(const Uncopyable& other) = delete;
  Uncopyable& operator=(const Uncopyable& other) = delete;
};
}  // namespace Peon

#endif