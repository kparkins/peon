#include "Timer.h"

#ifdef _WIN32
Peon::Timer::Timer() {
    QueryPerformanceFrequency(&mFrequency);
    mBegin.QuadPart = 0;
    mEnd.QuadPart = 0;
    mRunning = false;
}

Peon::Timer::~Timer() {
}

void Peon::Timer::Start() {
    mRunning = true;
    QueryPerformanceCounter(&mBegin);
}

void Peon::Timer::Stop() {
    QueryPerformanceCounter(&mEnd);
    mRunning = false;
}

double Peon::Timer::GetElapsedSec() {
    return this->CalculateElapsedTime(1.0);
}

double Peon::Timer::GetElapsedMillis() {
    return this->CalculateElapsedTime(1000.0);
}

double Peon::Timer::GetElapsedMicros() {
    return this->CalculateElapsedTime(1000000.0);
}

#else

Peon::Timer::Timer() {
    mBegin.tv_sec = 0;
    mBegin.tv_usec = 0;
    mEnd.tv_sec = 0;
    mEnd.tv_usec = 0;
    mRunning = false;
}

Peon::Timer::~Timer() {
}

void Peon::Timer::Start() {
    mRunning = true;
    gettimeofday(&mBegin, NULL);
}

void Peon::Timer::Stop() {
    gettimeofday(&mEnd, NULL);
    mRunning = false;
}

double Peon::Timer::GetElapsedSec() {
    return this->CalculateElapsedTime(1.0, 1000000.0);
}

double Peon::Timer::GetElapsedMillis() {
    return this->CalculateElapsedTime(1000.0, 1000.0);
}

double Peon::Timer::GetElapsedMicros() {
    return this->CalculateElapsedTime(1000000.0, 1.0);
}

#endif
