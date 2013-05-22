#include <iostream>
#include "tester.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>


using namespace std;

Test::Test(string description, std::function<int(testLogger& log)> test) {
    this->testLogged = test;
    this->testType = LOGGED;
    this->description = description;
}

void Test::RunTest() {
    cout << description << ": ";
    testLogger log;
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
        cout << "Log follows: " << endl;
        cout << log.str() << endl;
        exit(result);
    }
}
