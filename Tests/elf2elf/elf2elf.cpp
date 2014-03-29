#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include <elf.h>
#include <memory>
#include <string>
#include "tester.h"

/**
 * Parse some example binaries to pick up memory corruption  
 * or other runtime errors. 
 * Used for debugging not vlaidating behaviour
 */

using namespace std;

static string inputFile ="";
static string outputFile ="";

int Elf2Elf(testLogger& log);

int main(int argc, const char *argv[])
{
    inputFile = "../elf2elf/isYes/isYes.o";
    outputFile = "/tmp/isYes.o";
    Test("Reading small object file",Elf2Elf).RunTest();
    inputFile = argv[0];
    outputFile = "/tmp/elf2el";
    Test("Reading smallish binary",Elf2Elf).RunTest();
    return 0;
}

int Elf2Elf(testLogger& log) {
    // Map the input into memorry
    ElfFileReader f(inputFile.c_str());
    // Create the output file
    OFStreamWriter of(outputFile.c_str());

    ElfParser p(f);
    
    ElfFile file( p.Content());

    file.WriteToFile(of);
    
    // Output the data
    BinaryReader readPos(BinaryWriter(of));
    
    return 0;
}
