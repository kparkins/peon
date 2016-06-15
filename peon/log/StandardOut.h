/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_STANDARD_OUT_H
#define PEON_STANDARD_OUT_H

#include <mutex>
#include <iostream>

#include "LogStream.h"

using std::cout;
using std::mutex;
using std::lock_guard;

namespace Peon {
    class StandardOut : public LogStream {
    public:

        StandardOut();
        ~StandardOut();

        void Write(const string &message) override;

    protected:

        static mutex mMutex;

    };
}

#endif // PEON_CONSOLE_STREAM_H
