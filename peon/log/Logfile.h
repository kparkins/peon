/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#ifndef PEON_LOG_FILE_STREAM_H
#define PEON_LOG_FILE_STREAM_H

#include <fstream>
#include <stdexcept>
#include <string>

#include "LogStream.h"

using std::ios_base;
using std::ofstream;
using std::runtime_error;
using std::string;

namespace Peon {
class LogFile : public LogStream {
 public:
  explicit LogFile(const string& file, ios_base::openmode mode = ios_base::out);
  ~LogFile();

  void Write(const string& message) override;
  bool IsOpen() const;

 protected:
  string mFilename;
  ofstream mOstream;
};
}  // namespace Peon
#endif  // PEON_LOG_FILE_STREAM_H
