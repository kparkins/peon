/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "Logfile.h"
#include "Logger.h"

Peon::LogFile::LogFile(const string & file, ios_base::openmode mode) {
    mFilename = file;
    mOstream.open(file, mode);
}

Peon::LogFile::~LogFile() {
    if (mOstream.is_open()) {
        mOstream.close();
    }
}

void Peon::LogFile::Write(const string &message) {
    if (mOstream.is_open() && mOstream.good()) {
        mOstream << message;
    }
}

bool Peon::LogFile::IsOpen() const {
    return mOstream.is_open();
}