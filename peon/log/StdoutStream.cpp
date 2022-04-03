/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "StdoutStream.h"

mutex Peon::StdoutStream::mMutex;

Peon::StdoutStream::StdoutStream() {

}

Peon::StdoutStream::~StdoutStream() {
 
}

void Peon::StdoutStream::Write(const string &message) {
    lock_guard<mutex> lock(mMutex);
    cout << message;
}
