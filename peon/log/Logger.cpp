/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "Logger.h"

unsigned int Peon::Logger::mNextLoggerId = 0;
Peon::Logger gLogger;

Peon::Logger::Logger() {
    mLoggerId = mNextLoggerId++;
    mLogLevel = LogLevel::WARNING;
}

Peon::Logger::~Logger() {
    mStreams.clear();
}

Peon::LogLevel Peon::Logger::GetLogLevel() {
    lock_guard<mutex> lock(mMutex);
    return this->mLogLevel;
}

void Peon::Logger::SetLogLevel(LogLevel level) {
    lock_guard<mutex> lock(mMutex);
    this->mLogLevel = level;
}

void Peon::Logger::AddStream(LogStream* stream) {
    lock_guard<mutex> lock(mMutex);
    assert(stream);
    mStreams.push_back(stream);
}

void Peon::Logger::RemoveStream(LogStream* stream) {
    lock_guard<mutex> lock(mMutex);
    remove_if(mStreams.begin(), mStreams.end(),
              [&] (const LogStream* p) -> bool {
                  return stream == p;
              });
}

void Peon::Logger::Log(const string & message) {
    lock_guard<mutex> lock(mMutex);
    for(auto stream : mStreams) {
        stream->Write(message);
    }
}
