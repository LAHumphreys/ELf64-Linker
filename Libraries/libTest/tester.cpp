#include <iostream>
#include "tester.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

Test::Test(string description, std::function<int(void)> test) {
    this->test = test;
	this->testType = BASIC;
    this->description = description;
}

Test::Test(string description, std::function<int(stringstream& log)> test) {
	this->testLogged = test;
	this->testType = LOGGED;
    this->description = description;
}

void Test::RunTest() {
	stringstream log;
	int result;
    cout << description << ": ";
	if ( testType == BASIC ) {
	    result = test();
	} else {
		result = testLogged(log);
	}
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
