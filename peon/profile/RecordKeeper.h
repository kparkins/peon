#ifndef PEON_PROFILE_RECORD_KEEPER_H
#define PEON_PROFILE_RECORD_KEEPER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "TimeRecord.h"
#include "event/Event.h"
#include "event/EventHandler.h"
#include "event/EventGroup.h"

using std::string;
using std::vector;
using std::unordered_map;

namespace Peon {
    
    class RecordKeeper {

    public:

        static void AddTimeRecord(const TimeRecord & record);
        static void Update();

    protected:

        static vector<TimeRecord> mNewTimeRecords;
        static unordered_map<string, TimeRecord> mTimeRecords;

    };

}

#endif