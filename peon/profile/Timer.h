#ifndef PEON_PROFILE_TIMER_H
#define PEON_PROFILE_TIMER_H

#ifdef _WIN32
#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h>
#undef ERROR
#else
#include <sys/time.h>
#endif

namespace Peon {

class Timer {
 public:
  Timer();
  ~Timer();

  void Start();
  void Stop();

  double GetElapsedSec();
  double GetElapsedMillis();
  double GetElapsedMicros();

 protected:
  bool mRunning;

#ifdef _WIN32

  inline double CalculateElapsedTime(double units) {
    if (mRunning) {
      QueryPerformanceCounter(&mEnd);
    }
    return ((mEnd.QuadPart - mBegin.QuadPart) * units) / mFrequency.QuadPart;
  }

  LARGE_INTEGER mBegin;
  LARGE_INTEGER mEnd;
  LARGE_INTEGER mFrequency;

#else

  inline double CalculateElapsedTime(double secUnit, double usecUnit) {
    if (mRunning) {
      gettimeofday(&mEnd, NULL);
    }
    return (mEnd.tv_sec * secUnit + mEnd.tv_usec / usecUnit) -
           (mBegin.tv_sec * secUnit + mBegin.tv_usec / usecUnit);
  }

  timeval mBegin;
  timeval mEnd;

#endif
};

}  // namespace Peon

#endif