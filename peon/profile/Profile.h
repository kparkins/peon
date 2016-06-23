#ifndef PEON_PROFILE_H
#define PEON_PROFILE_H

#include <chrono>
#include <string>

#include "log/Logger.h"
#include "common/Macros.h"

using std::string;
using std::chrono::high_resolution_clock;

// TODO make this actually useful. Collate records / do actual timings. Figure out how to route it to
// a place that can render the info as text
namespace Peon {
    
    struct TimeRecord {
        long long elapsedTime;
    };

    template<int blockId> 
    class BlockTimer {
    
    public:

        BlockTimer(const string & file, const string & function);
        ~BlockTimer();

    private:

        unsigned int mBlockId;
        
    };
}

#define PEON_TIMED_BLOCK BlockTimer<__COUNTER__> blockTimer##__COUNTER__(__FILENAME__,  __FUNCTION__);

#endif

template<int blockId>
Peon::BlockTimer<blockId>::BlockTimer(const string & file, const string & function) {
    mBlockId = blockId;
    std::cout << "t construct " << blockId << " " << file << " " << function << std::endl;
}

template<int blockId>
Peon::BlockTimer<blockId>::~BlockTimer() {
    std::cout << "t destruct " << mBlockId << std::endl;
}