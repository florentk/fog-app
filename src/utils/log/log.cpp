/****************************************************************************/
/// @file    log.cpp
/// @author  Julen Maneros
/// @date
/// @version $Id:
///
// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <time.h>
#include <sstream>
#include <cstdlib>

#include "log.h"
#include "../../utils/common/TplConvert.h"

using namespace std;

Log* Log::instance_ = 0;
long Log::lineCounter_ = 0;
LogLevel Log::logLevel_;
long Log::timeStepThreshold_;
long Log::nextTimeStepThreshold_;
int Log::currentNumberLogFiles_ = 0;

Log::Log(string path, long timeThreshold)
{
    path_ = path;
    myfile_.open(path.c_str());
    logLevel_ = kLogLevelInfo;
    timeStepThreshold_ = timeThreshold;
    nextTimeStepThreshold_ = timeStepThreshold_ - 1;
}

int
Log::StartLog(string path)
{
    if (instance_ == 0) {
        instance_ = new Log(path, 2);
        return EXIT_SUCCESS;
    }

    return -1;
}

void
Log::SetLogLevel(LogLevel logLevel)
{
    logLevel_ = logLevel;
}

void
Log::SetLogTimeThreshold(char* logThreshold)
{
    try {
        timeStepThreshold_ = TplConvert<char>::_2long(logThreshold);
    } catch (EmptyData e) {
        cout << "[INFO] Log time files maximum timestep set to 200."<< endl;
        timeStepThreshold_ = 200;
    }
}

void
Log::SetLogTimeThreshold(string logThreshold)
{
    try {
        timeStepThreshold_ = TplConvert<char>::_2long(logThreshold.c_str());
    } catch (EmptyData e) {
        cout << "[INFO] Log time files maximum timestep set to 200."<< endl;
        timeStepThreshold_ = 200;
    }
}

void
Log::Close()
{
    instance_->myfile_.close();
    delete instance_;
}

bool
Log::Write(const char* message, LogLevel messageLogLevel)
{
    // decides if the message will be written or not
    bool write = false;
    string levelName;

    // If level set to INFO write ALL messages
    if (logLevel_ == kLogLevelInfo) {
        write = true;
        levelName = "[INFO] ";
    }

    // If level set to WARNING write except if message leve is is INFO
    if (logLevel_ == kLogLevelWarning && messageLogLevel != kLogLevelInfo) {
        write = true;
        levelName = "[WARNING] ";
    }

    // If level set to ERROR and message IS ERROR write
    if ((logLevel_ == kLogLevelError) && (messageLogLevel == kLogLevelError))  {
        write = true;
        levelName = "[ERROR] ";
    }

    if (!write)
        return true; // Writing is canceled

    if (!instance_->myfile_.good()) {
        return false;
    }
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (timeinfo == NULL) {
        cout << "[WARNING] Impossible to get time from system." << endl;
        return false;
    }

    // Removes \0 character
    stringstream auxi;
    auxi.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
    auxi << asctime(timeinfo);
    string mytime;
    try {
        getline(auxi, mytime);
    } catch (stringstream::failure e) {
        cout << "[WARNING] Impossible to format system time." << endl;
        return false;
    }

    instance_->myfile_ << "[" << mytime << "] " << levelName << message << endl;

    lineCounter_++;

    return true;
}

bool
Log::WriteHeader(const char* message)
{
    if (!instance_->myfile_.good()) {
        return false;
    }

    instance_->myfile_  << message << endl;
    instance_->myfile_ << "===============================" << endl;

    lineCounter_++;
    lineCounter_++;

    return true;
}

string
Log::GetPath()
{
    return instance_->path_;
}