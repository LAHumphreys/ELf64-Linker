#include <iostream>
#include "tester.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>


using namespace std;

Test::Test(string description, std::function<int(void)> test) {
    this->test = test;
	this->testType = BASIC;
    this->description = description;
}

Test::Test(string description, std::function<int(testLogger& log)> test) {
	this->testLogged = test;
	this->testType = LOGGED;
    this->description = description;
}

void Test::RunTest() {
    cout << description << ": ";
	testLogger log;
    #ifdef __PROFILE__TESTS
        auto start = chrono::high_resolution_clock::now();
    #endif

	int result;
	if ( testType == BASIC ) {
	    result = test();
	} else {
		result = testLogged(log);
	}
    #ifdef __PROFILE__TESTS
        auto stop = chrono::high_resolution_clock::now();
        if (result == 0 ) {
            long TICKS_PER_SECOND = 100000000;
            double duration = chrono::duration_cast<chrono::duration<double> >(stop - start).count();
            if ( duration == 0 ) {
                duration = 1.0 / TICKS_PER_SECOND;
            }
            long target = 60 * TICKS_PER_SECOND; 
            long dur    = duration * TICKS_PER_SECOND +1;
            cout << duration << " , "  << target << " , " << dur << endl;
            for ( long t = dur; t < target; t+=dur )
            {
                testLogged(log);
            }

        }
    #endif
    if (result == 0 ) {
        cout << "TEST PASSED" << endl;
    } else {
        cout << "TEST FAILED" << endl;
		if ( testType == LOGGED ) {
			cout << "Log follows: " << endl;
			cout << log.str() << endl;
		}
        exit(result);
    }
}
