#ifndef TESTER_H
#define TESTER_H
#include <functional>
#include <string>
#include <sstream>

using namespace std;

class dummyLog
{
public:
     // Dummy constructor
     dummyLog() {}
     dummyLog(const basic_stringstream<char>&rhs) {}

     template<class T>
     inline dummyLog& operator<<(const T& a) { return *this;}
     template<class T>
     inline dummyLog& operator<<(const char *) { return *this;}
     inline string str() const { return ""; }

     // Handle endl
     static dummyLog& endl(dummyLog& stream)
     {
         return stream;
     }

     // Handle endl
     static dummyLog endl(basic_stringstream<char>& stream)
     {
         return stream;
     }

     typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    // this is the function signature of std::endl
    typedef CoutType& (*StandardManip)(CoutType&);

    // define an operator<< to take in std::endl
    dummyLog& operator<<(StandardManip manip)
    {
        // Drop the manipulation, and just return the stream
        return *this;
    }
};

#ifdef __PROFILE__TESTS
    typedef dummyLog testLogger;
#else
    typedef stringstream testLogger;
#endif

enum TEST_TEMPLATE { BASIC, LOGGED };
typedef std::function<int(void)> test;
typedef std::function<int(testLogger&)> loggedTest ;

class Test {
public:
    Test (string description, std::function<int(void)> test);
    Test (string description, std::function<int(testLogger&)> test);
    void RunTest();

private:
    std::function<int(void)> test;
    std::function<int(testLogger& )> testLogged;
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
