#include <iostream>
#include "tester.h"
#include <iomanip>

using namespace std;

Test::Test(string description, std::function<int(void)> test) {
    this->test = test;
    this->description = description;
}

void Test::RunTest() {
    cout <<setw(80)<<setfill('*')<< "***" << endl;
    cout << description << endl;
    int result = test();
    if (result == 0 ) {
        cout << "TEST PASSED" << endl;
    } else {
        cout << "TEST FAILED" << endl;
        exit(result);
    }
    cout <<setw(80)<<setfill('*')<< "***" << endl;
}
