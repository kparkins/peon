#include "RecordKeeper.h"

vector<Peon::TimeRecord> Peon::RecordKeeper::mNewTimeRecords;
unordered_map<string, Peon::TimeRecord> Peon::RecordKeeper::mTimeRecords;

void Peon::RecordKeeper::AddTimeRecord(const TimeRecord & record) {
    mNewTimeRecords.push_back(record);
}

void Peon::RecordKeeper::Update() {
    mNewTimeRecords.clear();
}