
/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_LOGGER_H
#define PEON_LOGGER_H

#include <ctime>
#include <mutex>
#include <thread>
#include <string>
#include <memory>
#include <vector>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "common/Macros.h"
#include "common/Utility.h"
#include "LogStream.h"
#include "CompileFlags.h"

using std::setw;
using std::left;
using std::time;
using std::endl;
using std::mutex;
using std::vector;
using std::gmtime;
using std::string;
using std::ostream;
using std::setfill;
using std::remove_if;
using std::unique_ptr;
using std::lock_guard;
using std::make_shared;
using std::stringstream;

namespace Peon {

        enum LogLevel {
            TRACE = 5,
            DEBUG = 3,
            INFO = 4,
            WARNING = 2,
            ERROR = 1,
            FATAL = 0
        };

        class Logger {
        public:

            Logger();
            ~Logger();

            LogLevel GetLogLevel();
            void SetLogLevel(LogLevel level);

            void AddStream(LogStream* stream);
            void RemoveStream(LogStream* stream);

            void Log(const string &message);

        private:

            mutex mMutex;
            LogLevel mLogLevel;
            vector<LogStream*> mStreams;
            unsigned int mLoggerId;
            static unsigned int mNextLoggerId;

        };


#if PEON_ENABLE_LOGGING
#define LOG(logger, level, tag, msg) \
    { \
        if(logger.GetLogLevel() >= level) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << Peon::ToHex(id) << "] " \
                    << Peon::GmtTimestamp()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << tag << msg << endl; \
            logger.Log(sstream.str()); \
        } \
    } \

#define LOG_TRACE(...) SELECT_MACRO(LOG_TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) SELECT_MACRO(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) SELECT_MACRO(LOG_INFO, __VA_ARGS__)
#define LOG_WARNING(...) SELECT_MACRO(LOG_WARNING, __VA_ARGS__)
#define LOG_ERROR(...) SELECT_MACRO(LOG_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) SELECT_MACRO(LOG_FATAL, __VA_ARGS__)

#define LOG_TRACE_1(msg) LOG(gLogger, LogLevel::TRACE, " [TRACE] ", msg)
#define LOG_DEBUG_1(msg) LOG(gLogger, LogLevel::DEBUG, " [DEBUG] ", msg)
#define LOG_INFO_1(msg) LOG(gLogger, LogLevel::INFO, " [INFO] ", msg);
#define LOG_WARNING_1(msg) LOG(gLogger, LogLevel::WARNING, " [WARNING] ", msg)
#define LOG_ERROR_1(msg) LOG(gLogger, LogLevel::ERROR, " [ERROR] ", msg)
#define LOG_FATAL_1(msg) LOG(gLogger, LogLevel::FATAL, " [FATAL] ", msg)

#define LOG_TRACE_2(logger, msg) LOG(logger, LogLevel::TRACE, " [TRACE] ", msg)
#define LOG_DEBUG_2(logger, msg) LOG(logger, LogLevel::DEBUG, " [DEBUG] ", msg)
#define LOG_INFO_2(logger, msg) LOG(logger, LogLevel::INFO, " [INFO] ", msg);
#define LOG_WARNING_2(logger, msg) LOG(logger, LogLevel::WARNING, " [WARNING] ", msg)
#define LOG_ERROR_2(logger, msg) LOG(logger, LogLevel::ERROR, " [ERROR] ", msg)
#define LOG_FATAL_2(logger, msg) LOG(logger, LogLevel::FATAL, " [FATAL] ", msg)

#else

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)

#endif

}

// global Logger
extern Peon::Logger gLogger;

#endif //GNUT_LOGGER_H
