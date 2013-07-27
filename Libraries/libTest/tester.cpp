#include <iostream>
#include "tester.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include "util_time.h"
#include "stdWriter.h"
#include "env.h"

DefaultTestLogger DefaultTestLogger::GLOBAL_LOG;

using namespace std;
DefaultTestLogger& DefaultTestLogger::RunTimeLog() {
    return GLOBAL_LOG;
}

DefaultTestLogger::DefaultTestLogger() {
    Logger::Instance().RegisterLog(*this);

    // Prevent Logging directly to stdout / stderr
    Logger::Instance().RemoveLog(LogFactory::CERR());
    Logger::Instance().RemoveLog(LogFactory::CLOG());
    Logger::Instance().RemoveLog(LogFactory::COUT());

    Logger::Instance().LogEnabled( LOG_DEFAULT, true);
    Logger::Instance().LogEnabled( LOG_OVERVIEW, true);
    Logger::Instance().LogEnabled( LOG_VERBOSE, true);
    Logger::Instance().LogEnabled( LOG_VERY_VERBOSE, true);
    Logger::Instance().LogEnabled( LOG_WARNING, true);
    Logger::Instance().LogEnabled( LOG_ERROR, true);
}

DefaultTestLogger::~DefaultTestLogger() {
    Logger::Instance().RemoveLog(*this);
}

void DefaultTestLogger::WriteLog(const string& fname, const Time& time) {
    OFStreamWriter("OverviewLog_" + time.FileTimestamp() + fname) << overview_log.str();
    OFStreamWriter("FullLog_" + time.FileTimestamp() + fname) << full_log.str();
}

void DefaultTestLogger::Log( const string& message,
                             const string& context, 
                             const Time& time,
                             LOG_LEVEL level) {
    if ( level == LOG_DEFAULT ) {
        testoutput << message <<endl;
        overview_log << message <<endl;
        full_log << message <<endl;

    } else if (    level == LOG_ERROR
                || level == LOG_WARNING
                || level == LOG_OVERVIEW) 
    {
        testoutput << GenericFormatLogger::Format(message,context,time,level);
        overview_log << GenericFormatLogger::Format(message,context,time,level);
        full_log << GenericFormatLogger::Format(message,context,time,level);
    } else {
        full_log << GenericFormatLogger::Format(message,context,time,level);

        if ( ENV::IsSet("ELF_64_TEST_FULL_TEST_LOG") ) {
            testoutput << GenericFormatLogger::Format(message,context,time,level);
        }
    }
}

Test::Test(string description, std::function<int(testLogger& log)> test) {
    this->testLogged = test;
    this->testType = LOGGED;
    this->description = description;
}

void Test::RunTest() {
    cout << description << ": ";
    testLogger log;
    startTime.SetNow();
    #ifdef __PROFILE__TESTS
        auto start = clock();
    #endif
    int result = testLogged(log);
    #ifdef __PROFILE__TESTS
        // we don't want to mess around in the invoke function
        using TestPtr = int(*)(testLogger&);
        TestPtr f = *testLogged.target<TestPtr>();
        int count =1;
        while ( clock() == start ) {
            f(log);
            count++;
        }
        auto stop = clock();
        if (result == 0 ) {
            long TICKS_PER_SECOND = CLOCKS_PER_SEC;
            double duration = stop +1 - start;
            long target = __PROFILE_CLOCK_SECS__ * TICKS_PER_SECOND; 

            cout << duration << " (x"  << count << "), " << target;
            cout << " , " << TICKS_PER_SECOND;
            cout << "( " << stop << " - " << start << " )" << endl;

            for ( long t = duration; t < target; t+=duration ) {
                for ( int i=0; i<count; i++) {
                    f(log);
                }
            }
        }
    #endif
    if (result == 0 ) {
        cout << "TEST PASSED" << endl;
    } else {
        cout << "TEST FAILED" << endl;
        cout << "Run Time Log:" << endl;
        cout << "Test Log follows: " << endl;
        cout << log.str() << endl;
        DefaultTestLogger::RunTimeLog().WriteLog("TestRuntimeLog", startTime);
        log.WriteLog(description, startTime);
        if ( ENV::IsSet("ELF_64_TEST_FULL_TEST_LOG") ) {
            cout << DefaultTestLogger::RunTimeLog().str() << endl;
        }
        exit(result);
    }
}
