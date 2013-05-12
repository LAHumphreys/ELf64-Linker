#include "dataVector.h"
#include "tester.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include "binaryReader.h"

using namespace std;

int ReaderOverload( testLogger& log);
int WriteOverload ( testLogger& log);
  
int main(int argc, const char *argv[])
{
    Test("Filling a vector works",  (loggedTest)ReaderOverload).RunTest();
    Test("Filling a vector works",  (loggedTest)WriteOverload).RunTest();
    return 0;
}

int ReaderOverload( testLogger& log) {
    DataVector data(50);
    BinaryReader read1(data);
    BinaryReader read2 = read1 + 5;

    return 0;
}

int WriteOverload( testLogger& log) {
    DataVector data(50);
    BinaryWriter write1(data);
    BinaryWriter write2 = write1 + 5;

    return 0;
}
