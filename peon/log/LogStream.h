/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_LOG_STREAM_H
#define PEON_LOG_STREAM_H

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

namespace Peon {
class LogStream {
 public:
  virtual ~LogStream() {}
  virtual void Write(const string& message) = 0;
};

}  // namespace Peon
#endif  // PEON_LOG_STREAM_H
