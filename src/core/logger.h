#pragma once
/*
 *   Copyright 2015 Andy Warner
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <fstream>

namespace aft
{
namespace base
{
    class Context;
}

namespace core
{

/**
 * Log levels
 */
typedef int AftLogLevel;

extern const int Trace;
extern const int Debug;
extern const int Info;
extern const int Warning;
extern const int Error;
extern const int Fatal;

enum AftLogType
{
    INVALID = -1,
    ALERT,
    AUDIT,
    LOG
};


class LogStreamBuf : public std::filebuf
{
public:
    LogStreamBuf(AftLogType logType);
    virtual ~LogStreamBuf();

    virtual int sync();

    AftLogLevel getLogLevel() const { return logLevel_; }
    void setLogLevel(AftLogLevel level) { logLevel_ = level; }

private:
    AftLogType  logType_;
    AftLogLevel logLevel_;

    unsigned int preambleSize_;
    char buf_[1024];
};

/**
 *  Utility class for logging
 */
class Logger : public std::ofstream
{
public:
    Logger(AftLogType logType, const std::string& file = std::string());
    virtual ~Logger();

    /**
     *  Set up standard loggers aftalert, aftaudit and aftlog based on a logging
     *  configuration.  This is typically called in a run context
     *
     * Note: This currently just take sthe string parameter as a prefix for the
     *       log file name.  It appends audit.log and log.log.
     *       This will be replaced with a LogConfig parameter.
     */
    static void initStandardLoggers(const std::string& logConfig);

    void setLogLevel(AftLogLevel level = Debug);
    void setLowLogLevel(AftLogLevel level = Debug);
    void writeAlert(const std::string& msg);
    void writeAudit(const std::string& msg);
    void writeLog(const std::string& msg);

private:
    void openLogFile(const std::string& file, bool buffered);

private:
    AftLogLevel lowLogLevel_;
    LogStreamBuf streamBuf_;
};


struct _Level { AftLogLevel level; };

_Level loglevel(AftLogLevel level = Debug);
Logger& operator<<(Logger& logger, _Level level);

extern Logger& aftalert;
extern Logger& aftaudit;
extern Logger& aftlog;

} // namespace core
} // namespace aft

