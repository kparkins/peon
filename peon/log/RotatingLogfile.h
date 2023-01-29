/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#ifndef PEON_ROTATING_LOGFILE_H
#define PEON_ROTATING_LOGFILE_H

#include <fstream>
#include <stdexcept>

#include "LogStream.h"

using std::fstream;
using std::ios_base;
using std::runtime_error;

namespace Peon {
class RotatingLogFile : public LogStream {
 public:
  RotatingLogFile(const string& prefix,
                  unsigned int maxFiles,
                  unsigned int maxLines,
                  ios_base::openmode mode = ios_base::out);
  ~RotatingLogFile();

  void SetMaxFiles(unsigned int n);
  unsigned int GetMaxFiles();

  void SetMaxLines(unsigned int l);
  unsigned int GetMaxLines();

  string GetPrefix();

  void Write(const string& message) override;

 protected:
  string mPrefix;
  fstream mLogfile;
  ios_base::openmode mMode;

  unsigned int mMaxFiles;
  unsigned int mMaxLines;
  unsigned int mFileIndex;
  unsigned int mLineIndex;
};
}  // namespace Peon

#endif  // GNUT_ROTATING_LOGFILE_H
