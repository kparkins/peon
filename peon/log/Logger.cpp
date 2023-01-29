/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#include "Logger.h"

unsigned int Peon::Logger::mNextLoggerId = 0;
Peon::Logger gLogger;

Peon::Logger::Logger() {
  mLoggerId = mNextLoggerId++;
  mLogLevel = LogLevel::WARNING;
}

Peon::Logger::~Logger() {
  for (auto stream : mStreams) {
    delete stream;
  }
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

void Peon::Logger::AddStream(Unique<LogStream> stream) {
  lock_guard<mutex> lock(mMutex);
  assert(stream.get());
  mStreams.push_back(stream.release());
}

void Peon::Logger::Log(const string& message) {
  lock_guard<mutex> lock(mMutex);
  for (auto stream : mStreams) {
    stream->Write(message);
  }
}

void Peon::Logger::RemoveStreams() {
  for (auto s : mStreams) {
    delete s;
  }
  mStreams.resize(0);
}