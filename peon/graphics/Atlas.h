#ifndef PEON_ATLAS_H
#define PEON_ATLAS_H

#include <string>
#include <unordered_map>

#include "common/Uncopyable.h"

using std::string;
using std::unordered_map;

namespace Peon {

template <typename T>
class Atlas : public Peon::Uncopyable {
 public:
  Atlas();
  virtual ~Atlas();

  T* Lookup(const string& resource);
  void Put(const string& key, T* item);

 protected:
  unordered_map<string, T*> mResources;
};

}  // namespace Peon

template <typename T>
Peon::Atlas<T>::Atlas() {}

template <typename T>
Peon::Atlas<T>::~Atlas() {
  for (auto& pair : mResources) {
    delete pair.second;
  }
}

template <typename T>
T* Peon::Atlas<T>::Lookup(const string& resource) {
  auto it = mResources.find(resource);
  if (it == mResources.end()) {
    return nullptr;
  }
  return it->second;
}

template <typename T>
void Peon::Atlas<T>::Put(const string& key, T* item) {
  auto it = mResources.find(key);
  if (it != mResources.end()) {
    delete it->second;
    mResources[key] = item;
  }
  mResources[key] = item;
}

#endif