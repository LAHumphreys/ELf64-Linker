#include "tester.h"
#include "binaryData.h"
#include "dataLump.h"
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

/*
 * Coverage of of HexCode which isn't covered by the elf2elf
 * tests
 *
 */

int HexCode(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Check hex code output",(loggedTest)HexCode).RunTest();
    return 0;
}

int HexCode(testLogger& log ) {
    unsigned char input[256];
    stringstream expectedOutput;
    char base [] = "0123456789ABCDEF";

    for ( int i=0; i < 16; i++ ) {
        for ( int j=0; j< 16; j++ ) {
            expectedOutput << base[i] << base[j];
        }
    }

    for (int i = 0; i < 256; i++) {
       input[i] = i;
    }

    DataIO f(input,256);
    Data d(BinaryReader(f),256);

    string output = d.HexCode();

    if ( output != expectedOutput.str() ) {
        log << "Hex code missmatch!" << endl;
        log << "Expected: " << expectedOutput.str() << endl;
        log << "Actual: " << output << endl;
        return 1;
    } 

    return 0;
}
