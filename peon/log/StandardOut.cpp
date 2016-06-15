/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "StandardOut.h"

mutex Peon::StandardOut::mMutex;

Peon::StandardOut::StandardOut(){

}

Peon::StandardOut::~StandardOut() {

}

void Peon::StandardOut::Write(const string &message) {
    lock_guard<mutex> lock(mMutex);
    cout << message;
}
