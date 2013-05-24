#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include <elf.h>
#include <memory>
#include <string>

using namespace std;

int main(int argc, const char *argv[])
{
    string inputFile ="";
    string outputFile ="";
    if ( argc != 3 ) {
        if ( argc == 2 ) {
            inputFile = argv[1];
            outputFile = inputFile + ".out";
        } else {
            cout << "Usage: elf2elf <input file> <output file=<input file>.out" << endl;
            return 1;
        }
    } else {
        inputFile = argv[1];
        outputFile = argv[2];
    }

    // Map the input into memorry
    ElfFileReader f(inputFile.c_str());
    // Create the output file
    OFStreamWriter of(outputFile.c_str());

    ElfParser p(f);
    
    ElfFile file( p.Content());

    file.WriteToFile(of);
    
    // Output the data
    BinaryReader readPos(BinaryWriter(of));

    cout << "Re-wrote " << inputFile << " to " << outputFile << endl;

    return 0;
}
