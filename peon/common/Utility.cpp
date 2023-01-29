/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#include "Utility.h"

string Peon::GmtTimestamp() {
  stringstream sstream;
  static mutex gmtMutex;
  lock_guard<mutex> gmtLock(gmtMutex);

  time_t tnow = time(NULL);
#ifdef _MSC_VER
  tm t;
  tm* tm = &t;
  gmtime_s(tm, &tnow);
#else
  tm* tm = gmtime(&tnow);
#endif
  if (!tm) {
    LOG_ERROR("Unable to get current system time.");
    return string();
  }
  sstream << setfill('0') << setw(2) << tm->tm_mon + 1 << "/" << setfill('0')
          << setw(2) << tm->tm_wday << "/" << setfill('0') << setw(4)
          << tm->tm_year + 1900 << " " << setfill('0') << setw(2) << tm->tm_hour
          << ":" << setfill('0') << setw(2) << tm->tm_min << ":" << setfill('0')
          << setw(2) << tm->tm_sec << "  ";
  return sstream.str();
}

vector<string> Peon::Split(const string& source, char delimeter) {
  string token;
  vector<string> tokens;
  stringstream s(source);
  while (getline(s, token, delimeter)) {
    tokens.push_back(token);
  }
  return tokens;
}

string Peon::ReadFile(const string& file) {
  stringstream sstream;
  ifstream fileStream(file);
  if (!fileStream.is_open() || !fileStream.good()) {
    return string("");
  }
  sstream << fileStream.rdbuf();
  return sstream.str();
}