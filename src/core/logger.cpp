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

////////////////////////////// TODO this needs a major rework.

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#include "logger.h"
using namespace aft;
using namespace aft::core;
using namespace std;


const int core::Trace = 0;
const int core::Debug = 1;
const int core::Info = 2;
const int core::Warning = 3;
const int core::Error = 4;
const int core::Fatal = 5;
static const char* const LevelLetter = "TDIWEF";
static const char* const LevelName[] =
{
    "Trace", "Debug", "Info", "Warning", "Error", "Fatal"
};

Logger& core::aftalert = *new Logger(ALERT);
Logger& core::aftaudit = *new Logger(AUDIT, "aftaudit.log");
Logger& core::aftlog = *new Logger(LOG /*, "aftlog.log"*/);

static unsigned int getCurrentTime(char* buf, size_t bufSize)
{
    time_t     now = time(0);
    struct tm  tstruct;
    tstruct = *localtime(&now);
    strftime(buf, bufSize, "%Y-%m-%d.%X  ", &tstruct);
    return (unsigned int)strlen(buf);
}


LogStreamBuf::LogStreamBuf(AftLogType logType)
    : logType_(logType)
    , logLevel_(Debug)
    , preambleSize_(24)
{
    if (logType == LOG) preambleSize_ = 0;

    setbuf(buf_ + preambleSize_, sizeof(buf_) - preambleSize_);
}

LogStreamBuf::~LogStreamBuf()
{

}


int LogStreamBuf::sync()
{
    //cout << hex << "before sync logstream, pbase=" << static_cast<void*>(pbase())
    //     << ", pptr= " << static_cast<void*>(pptr())
    //     << ", epptr=" << static_cast<void*>(epptr()) << endl;

    // Send non-debug logs to audit, too
    if (logType_ == LOG && logLevel_ > Debug)
    {
        aftaudit.setLogLevel(logLevel_);
        aftaudit.write(buf_, pptr() - buf_);
        aftaudit.flush();
    }

    if (preambleSize_ > 20)
    {
        //TODO peek that last char is newline
        char timestamp[preambleSize_ + 1];
        unsigned int szPreamble = getCurrentTime(timestamp, preambleSize_);
        strncpy(buf_, timestamp, szPreamble);
        memset(&buf_[szPreamble], ' ', preambleSize_ - szPreamble);
        if (logLevel_ >= Trace && logLevel_ <= Fatal)
        {
            buf_[szPreamble - 1] = LevelLetter[logLevel_];
        }

        int savePos = pptr() - buf_;
        setp(buf_, epptr());
        pbump(savePos);
    }
    //cout << hex << "after sync logstream, pbase=" << static_cast<void*>(pbase())
    //     << ", pptr= " << static_cast<void*>(pptr())
    //     << ", epptr=" << static_cast<void*>(epptr()) << endl;

    int ret = filebuf::sync();

    if (preambleSize_ > 20)
    {
        setp(buf_ + preambleSize_, epptr());
    }

    setLogLevel(Debug);    // Reset to default loglevel

    return ret;
}


Logger::Logger(AftLogType logType, const std::string& file)
    : lowLogLevel_(Debug)
    , streamBuf_(logType)
{
    openLogFile(file, logType != INVALID);
}

void
Logger::openLogFile(const std::string& file, bool buffered)
{
    string logFile;
    if (file.empty())
    {
        logFile = "/dev/tty";
    } else {
        if (file[0] != '/' || file[0] != '.')
        {
            const char* home = getenv("HOME");
            if (!home || !home[0]) home = "/tmp";
            logFile = string(home) + "/logs/" + file;
        } else {
            logFile = file;
        }
    }

    if (buffered)
    {
        if (streamBuf_.is_open()) streamBuf_.close();

        if (!streamBuf_.open(logFile.c_str(), ios::out))
        {
            cerr << "Could not open file " << logFile << ".  Using /dev/null instead." << endl;
            streamBuf_.open("/dev/null", ios::out);
        }
        ostream::rdbuf(&streamBuf_);
    } else {
        if (is_open()) close();

        open(logFile.c_str());
        if (!is_open())
        {
            open("/dev/null");
        }
    }
}

Logger::~Logger()
{
    close();
}

void Logger::initStandardLoggers(const std::string& logConfig)
{
// aftalert logger remains as tty  *new Logger(ALERT)
    if (aftaudit.is_open()) aftaudit.close();
    aftaudit.openLogFile(logConfig + "audit.log", true);

    if (aftlog.is_open()) aftlog.close();
    aftlog.openLogFile(logConfig + "log.log", true);
}

void Logger::setLogLevel(core::AftLogLevel level)
{
    streamBuf_.setLogLevel(level);
}

void Logger::setLowLogLevel(core::AftLogLevel level)
{
    lowLogLevel_ = level;
}

void Logger::writeAlert(const std::string& msg)
{
    write("ALERT: ", 7);
    *this << msg << endl;
}

void Logger::writeAudit(const std::string& msg)
{
    write("AUDIT: ", 7);
    *this << msg << endl;
}

void Logger::writeLog(const std::string& msg)
{
    *this << msg << endl;
}


_Level core::loglevel(AftLogLevel level)
{
    _Level ret = { level };
    return ret;
    //return { level }; this is for C++0x
}

Logger& core::operator<<(Logger& logger, _Level level)
{
    logger.setLogLevel(level.level);
    return logger;
}
