#include "BlockTimer.h"

Peon::BlockTimer::BlockTimer(const char* file, const char* function, int line) {
  mRecord.file = file;
  mRecord.function = function;
  mRecord.line = static_cast<unsigned int>(line);
  mTimer.Start();
}

Peon::BlockTimer::~BlockTimer() {
  mTimer.Stop();
  mRecord.elapsedTime = mTimer.GetElapsedMicros();
  std::cout << mRecord.file << " - " << mRecord.function << " - "
            << mRecord.elapsedTime / 1000 << std::endl;
}
