#ifndef PEON_PROFILE_TIME_RECORD_H
#define PEON_PROFILE_TIME_RECORD_H

#include "event/Event.h"

namespace Peon {

    typedef struct TimeRecord {
        double elapsedTime;
        const char* file;
        const char* function;
        unsigned int line;
    }TimeRecord;

}

#endif