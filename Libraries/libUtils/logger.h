#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <set>
#include "binaryWriter.h"
#include <vector>

class Time;

/*
 * Provides a basic logging utility:
 * 
 *     LOG( LOG_OVERVIEW, "Hello World!")
 *
 * By default logging is sent to clog. This behaviour can be changed by removing 
 * the clog device, and adding your own:
 *
 *     OFStreamWriter logfile("MyLog.txt");
 *     LogDevice_BinaryWriter logger(logfile);
 *
 *     Logger.RemoveLog(LogFactory::CLOG());
 *     Logger.RegisterLog(logger);
 *
 *  Each registered device is passed a Time object, the log level, a context string,
 *  and the message itself.
 *
 *  The Factory objects, and the default Writer object print a standard timestamp header,
 *  and all messages sent to them
 *
 */

// Expressions for logging messages
#define PRINT(text) if ( Logger::LogEnabled(LOG_DEFAULT) ) { Logger::LogMessage(text, LOG_DEFAULT); }
#define LOG(level, text) if ( Logger::LogEnabled(level) ) { Logger::LogMessage(text, level); }
#define LOG_FROM(level, text, context) if ( Logger::LogEnabled(level) ) { Logger::LogMessage(text, level, context); }

// Valid Log Levels
using namespace std; 
enum LOG_LEVEL {
    LOG_DEFAULT,
    LOG_OVERVIEW,
    LOG_VERBOSE,
    LOG_VERY_VERBOSE,
    LOG_WARNING,
    LOG_ERROR,
    __NUM_LOG_LEVELS
};


/*
 * Class to be implemented to add a new log device
 */
class LogDevice { 
    friend class LogDeviceKey;
public:
    LogDevice();
    // The pointer is good only for the lifetime of the call
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) = 0;

    virtual ~LogDevice(){};


private:
    unsigned long LogDevice_Id;

    // Prevent more then one copy of the same ID
    LogDevice& operator=(const LogDevice& rhs) = delete;
    LogDevice(const LogDevice& rhs) = delete;
};

/*
 * Common instances
 */
class LogDevice_BinaryWriter: public LogDevice { 
    LogDevice_BinaryWriter(BinaryWriter& w): log(w){}

    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level);

private:
    BinaryWriter& log;
};

class LogFactory {
public:
    static LogDevice& COUT();
    static LogDevice& CERR();
    static LogDevice& CLOG();
};

class GenericFormatLogger { 
public:
    static string Format( const string& message,
                          const string& context,
                          const Time& time,
                          LOG_LEVEL level);
};



/*
 * Main Logging Class
 */
class Logger {
public:
    Logger ();
    virtual ~Logger (){}

    static bool LogEnabled(LOG_LEVEL level) {
        return enabled[level];
    }

    static void RegisterLog(LogDevice& device);
    static void RemoveLog(LogDevice& device);

    static void LogMessage( const string& message, 
                            LOG_LEVEL level,
                            const string& context="Log Message");
    static void LogMessage( const BinaryReader& msg, 
                            LOG_LEVEL level,
                            const string& context="Log Message");

private:  


    static bool enabled[__NUM_LOG_LEVELS];
};




#endif
