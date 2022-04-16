#ifndef PEON_PROFILE_RECORD_KEEPER_H
#define PEON_PROFILE_RECORD_KEEPER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "TimeRecord.h"
#include "event/Event.h"

using std::string;
using std::unordered_map;
using std::vector;

namespace Peon {

class RecordKeeper {
 public:
  static void AddTimeRecord(const TimeRecord& record);
  static void Update();

 protected:
  static vector<TimeRecord> mNewTimeRecords;
  static unordered_map<string, TimeRecord> mTimeRecords;
};

}  // namespace Peon

#endif