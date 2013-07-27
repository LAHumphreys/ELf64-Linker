#include "logger.h"
#include "env.h"
#include <iostream>
#include <string>
#include <sstream>
#include <util_time.h>

using namespace std;

/*
 * Wrapper class for use in set
 */
class LogDeviceKey {

public:
    LogDeviceKey(LogDevice& logDevice): log(&logDevice){}

    void Log( const string& message, 
              const string& context, 
              const Time& time,
              LOG_LEVEL level) const {
        log->Log(message, context ,time, level);
    }

    // Comparator
    class Less {
    public:
        bool operator()( const LogDeviceKey& lhs, 
                         const LogDeviceKey& rhs) {
            return   lhs.log->LogDevice_Id 
                   < rhs.log->LogDevice_Id;
        }
    };
private:
   LogDevice* log;
};


/*
 * Static Data
 */

bool Logger::enabled[__NUM_LOG_LEVELS];


LogDevice::LogDevice() {
    static unsigned long nextId =0;
    this->LogDevice_Id = nextId;
    ++nextId;
}

/*
 * Log Factory
 */
class LogDevice_COUT: public LogDevice {
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) {

        if ( level == LOG_DEFAULT ) {
            cout << message << endl;
        } else {
            cout << GenericFormatLogger::Format(message,context,time,level);
        }
    }
} _COUT;

class LogDevice_CERR: public LogDevice {
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) {

        if ( level == LOG_DEFAULT ) {
            cout << message << endl;
        } else {
            cout << GenericFormatLogger::Format(message,context,time,level);
        }
    }
} _CERR;

class LogDevice_CLOG: public LogDevice {
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) {

        if ( level == LOG_DEFAULT ) {
            cout << message << endl;
        } else {
            cout << GenericFormatLogger::Format(message,context,time,level);
        }
    }
} _CLOG;

LogDevice& LogFactory::COUT() {
    return _COUT;
}

LogDevice& LogFactory::CERR() {
    return _CERR;
}

LogDevice& LogFactory::CLOG() {
    return _CLOG;
}

/*
 * Internal Data
 */
namespace {
    set<LogDeviceKey,LogDeviceKey::Less> devices;
    vector<string> logLevelNames;
    Logger GLOBAL_LOGGER;
    LogDevice_COUT _COUT;
    LogDevice_CLOG _CLOG;
    LogDevice_CERR _CERR;
}




/*
 * Logger Implementation
 */

Logger::Logger() {
    logLevelNames.resize(__NUM_LOG_LEVELS);

    logLevelNames[LOG_DEFAULT] = "STDOUT";
    logLevelNames[LOG_OVERVIEW] = "DEBUG 1";
    logLevelNames[LOG_VERBOSE] = "DEBUG 2";
    logLevelNames[LOG_VERY_VERBOSE] = "DEBUG 3";
    logLevelNames[LOG_WARNING] = "WARNING";
    logLevelNames[LOG_ERROR] = "ERROR";

    enabled[LOG_DEFAULT]      = true;
    enabled[LOG_WARNING]      = true;
    enabled[LOG_ERROR]        = true;

    enabled[LOG_OVERVIEW]     = ENV::IsSet("ELF_64_LINK_LOG_OVERVIEW");
    enabled[LOG_VERBOSE]      = ENV::IsSet("ELF_64_LINK_LOG_VERBOSE");
    enabled[LOG_VERY_VERBOSE] = ENV::IsSet("ELF_64_LINK_LOG_VERY_VERBOSE");


    RegisterLog(LogFactory::CLOG());
}

void Logger::RegisterLog(LogDevice& log) {
    devices.insert(log);
}

void Logger::RemoveLog(LogDevice& log) {
    devices.erase(log);
}


void Logger::LogMessage( const string& message, 
                         LOG_LEVEL level,
                         const string& context) {
    for(const LogDeviceKey& device: devices) {
        device.Log(message, context, Time(), level );
    }
}

void Logger::LogMessage( const BinaryReader& msg, 
                         LOG_LEVEL level,
                         const string& context) {
    string&& text = msg.ReadString();
    LogMessage(text,level, context);
}

void LogDevice_BinaryWriter::Log( const string& message,
                  const string& context, 
                  const Time& time,
                  LOG_LEVEL level) {
    log << GenericFormatLogger::Format(message,context,time,level);
}

string GenericFormatLogger::Format( const string& message,
                                    const string& context,
                                    const Time& time,
                                    LOG_LEVEL level) {
    stringstream log;
    log << "\n";
    log << "[" << logLevelNames[level] << "] " 
        << Time().Timestamp() << " - " 
        << context << ": " << "\n";
    log << message << "\n";
    log << "\n";
    return log.str();
}

