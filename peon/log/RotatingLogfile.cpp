/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#include "RotatingLogfile.h"

Peon::RotatingLogFile::RotatingLogFile(const string &prefix, unsigned int maxFiles,
    unsigned int maxLines, ios_base::openmode mode)
    : mPrefix(prefix),
    mMode(mode),
    mMaxFiles(maxFiles),
    mMaxLines(maxLines),
    mFileIndex(0),
    mLineIndex(0) {
    stringstream sstream;
    sstream << mPrefix << "-" << time(0);
    mPrefix = sstream.str();
    sstream << "-" << mFileIndex++ << ".log";

    mLogfile.open(sstream.str(), mode);
    if (!mLogfile.is_open() || !mLogfile.good()) {
        throw runtime_error("Unable to open rotating LogFile " + mPrefix);
    }
}

Peon::RotatingLogFile::~RotatingLogFile() {
    if (mLogfile.is_open()) {
        mLogfile.close();
    }
}

void Peon::RotatingLogFile::SetMaxFiles(unsigned int n) {
    mMaxFiles = n;
}

unsigned int Peon::RotatingLogFile::GetMaxFiles() {
    return mMaxFiles;
}

void Peon::RotatingLogFile::SetMaxLines(unsigned int l) {
    mMaxLines = l;
}

unsigned int Peon::RotatingLogFile::GetMaxLines() {
    return mMaxLines;
}

string Peon::RotatingLogFile::GetPrefix() {
    return mPrefix;
}

void Peon::RotatingLogFile::Write(const string & message) {
    if (mLineIndex >= mMaxFiles) {
        stringstream sstream;
        sstream << mPrefix << "-" << mFileIndex++ % mMaxFiles << ".log";
        mLogfile.close();
        mLogfile.open(sstream.str(), mMode);
        mLineIndex = 0;
    }

    if (mLogfile.is_open() && mLogfile.good()) {
        mLogfile << message;
        ++mLineIndex;
    }
}
