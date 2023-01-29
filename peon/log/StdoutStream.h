/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#ifndef PEON_STD_OUT_STREAM_H
#define PEON_STD_OUT_STREAM_H

#include <mutex>
#include <iostream>

#include "LogStream.h"

using std::cout;
using std::mutex;
using std::lock_guard;

namespace Peon {
    class StdoutStream : public LogStream {
    public:

        StdoutStream();
        ~StdoutStream();

        void Write(const string &message) override;

    protected:

        static mutex mMutex;

    };
}

#endif // PEON_CONSOLE_STREAM_H
