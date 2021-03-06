/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_ROTATING_LOGFILE_H
#define PEON_ROTATING_LOGFILE_H

#include <fstream>
#include <stdexcept>

#include "LogStream.h"

using std::ios_base;
using std::fstream;
using std::runtime_error;

namespace Peon {
    class RotatingLogFile : public LogStream {
    public:

        RotatingLogFile(const string & prefix, unsigned int maxFiles, unsigned int maxLines,
            ios_base::openmode mode = ios_base::out);
        ~RotatingLogFile();

        void SetMaxFiles(unsigned int n);
        unsigned int GetMaxFiles();

        void SetMaxLines(unsigned int l);
        unsigned int GetMaxLines();

        string GetPrefix();

        void Write(const string & message) override;

    protected:

        string mPrefix;
        fstream mLogfile;
        ios_base::openmode mMode;

        unsigned int mMaxFiles;
        unsigned int mMaxLines;
        unsigned int mFileIndex;
        unsigned int mLineIndex;

    };
}

#endif //GNUT_ROTATING_LOGFILE_H
