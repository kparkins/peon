/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#ifndef PEON_LOG_STREAM_H
#define PEON_LOG_STREAM_H

#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

using std::string;
using std::stringstream;

namespace Peon {
    class LogStream {
    public:

        virtual ~LogStream() { }
        virtual void Write(const string &message) = 0;

    };

}
#endif //PEON_LOG_STREAM_H
