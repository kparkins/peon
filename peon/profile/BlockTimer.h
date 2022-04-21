#ifndef PEON_PROFILE_BLOCK_TIMER_H
#define PEON_PROFILE_BLOCK_TIMER_H

#include <string>
#include <unordered_map>

#include "RecordKeeper.h"
#include "TimeRecord.h"
#include "Timer.h"
#include "common/Macros.h"
#include "log/Logger.h"

using std::hash;
using std::move;
using std::unordered_map;

namespace Peon {

class BlockTimer {
 public:
  BlockTimer(const char* file, const char* function, int line);
  ~BlockTimer();

 private:
  Timer mTimer;
  TimeRecord mRecord;
};
}  // namespace Peon

#define PEON_TIMED_BLOCK                                               \
  Peon::BlockTimer blockTimer##__COUNTER__(__FILENAME__, __FUNCTION__, \
                                           __LINE__);

#endif
