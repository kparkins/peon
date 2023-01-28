/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_STD_OUT_STREAM_H
#define PEON_STD_OUT_STREAM_H

#include <iostream>
#include <mutex>

#include "LogStream.h"

using std::cout;
using std::lock_guard;
using std::mutex;

namespace Peon {
class StdoutStream : public LogStream {
 public:
  StdoutStream();
  ~StdoutStream();

  void Write(const string& message) override;

 protected:
  static mutex mMutex;
};
}  // namespace Peon

#endif  // PEON_CONSOLE_STREAM_H
