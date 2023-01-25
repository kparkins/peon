#include "Bus.h"

Peon::Bus::~Bus() {
  for (auto pair : mBus) {
    delete pair.second;
  }
  mBus.clear();
}