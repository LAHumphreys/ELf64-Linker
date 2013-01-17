#ifndef TESTER_H
#define TESTER_H
#include <functional>
#include <string>

using namespace std;

enum TEST_TEMPLATE { BASIC, LOGGED };
typedef std::function<int(void)> test;
typedef std::function<int(stringstream&)> loggedTest ;

class Test {
public:
    Test (string description, std::function<int(void)> test);
    Test (string description, std::function<int(stringstream&)> test);
    void RunTest();

private:
    std::function<int(void)> test;
    std::function<int(stringstream& )> testLogged;
    string description;
	TEST_TEMPLATE testType;
};


template<class T>
void FillContainer(T& container, int size, char filler) {
    for (int i = 0; i < size; i++) {
        container[i]  = filler;
    }
}

#endif
