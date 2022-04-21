#ifndef GAME_ATLAS_H
#define GAME_ATLAS_H

#include <string>
#include <unordered_map>

#include "common/Uncopyable.h"

using std::string;
using std::unordered_map;

template <typename T>
class Atlas : public Peon::Uncopyable {
 public:
  Atlas() {}

  virtual ~Atlas() {
    for (auto& pair : mResources) {
      delete pair.second;
    }
  }

  T* Lookup(const string& resource) {
    auto it = mResources.find(resource);
    if (it == mResources.end()) {
      return nullptr;
    }
    return it->second;
  }

  void Put(const string& key, T* item) {
    auto it = mResources.find(key);
    if (it != mResources.end()) {
      delete it->second;
      mResources[key] = item;
    }
    mResources[key] = item;
  }

 protected:
  unordered_map<string, T*> mResources;
};

#endif