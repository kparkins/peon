/**
 * Copyright Kyle Parkinson. All rights reserved.
 */

#ifndef PEON_LOG_FILE_STREAM_H
#define PEON_LOG_FILE_STREAM_H

#include <string>
#include <fstream>
#include <stdexcept>

#include "LogStream.h"

using std::string;
using std::ios_base;
using std::ofstream;
using std::runtime_error;

namespace Peon {
    class LogFile : public LogStream {
    public:

        explicit LogFile(const string & file, ios_base::openmode mode = ios_base::out);
        ~LogFile();

        void Write(const string &message) override;
        bool IsOpen() const;

    protected:

        string mFilename;
        ofstream mOstream;

    };
}
#endif //PEON_LOG_FILE_STREAM_H
