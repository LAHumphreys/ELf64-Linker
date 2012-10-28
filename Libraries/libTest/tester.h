#ifndef TESTER_H
#define TESTER_H
#include <functional>
#include <string>

using namespace std;

class Test {
public:
    Test (string description, std::function<int(void)> test);
    void RunTest();

private:
    std::function<int(void)> test;
    string description;
};

template<class T>
void FillContainer(T& container, int size, char filler) {
    for (int i = 0; i < size; i++) {
        container[i]  = filler;
    }
}

#endif
