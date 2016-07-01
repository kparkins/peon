#ifndef PEON_PROFILE_BLOCK_TIMER_H
#define PEON_PROFILE_BLOCK_TIMER_H

#include <string>
#include <unordered_map>

#include "Timer.h"
#include "TimeRecord.h"
#include "log/Logger.h"
#include "common/Macros.h"
#include "event/EventDispatcher.h"
#include "RecordKeeper.h"

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
}

#define PEON_TIMED_BLOCK BlockTimer blockTimer##__COUNTER__(__FILENAME__,  __FUNCTION__, __LINE__);

#endif

