#include "binaryReader.h"
#include "elfHeader.h"
#include "elfReader.h"
#include "programHeader.h"
#include "stdReader.h"
#include "stdWriter.h"

#include <fstream>
#include <iostream>

using namespace std;
int main(int argc, const char *argv[])
{
    if ( argc  != 2 ) {
        cout << "Usage: addExec <binary file>" << endl;
        return -1;
    }
    // Open the file, and make IO interface objects
    fstream stdFile(argv[1], ios_base::in | ios_base::out | ios_base::binary);
    StdReader fileReader(stdFile);
    StdWriter fileWriter(stdFile);

    // Initialise the linker library objects
    ElfHeaderX86_64 header(fileReader);
    BinaryReader progReader (fileReader, header.ProgramHeadersStart());
    BinaryWriter progWriter (fileWriter, header.ProgramHeadersStart());

    // Stream the program headers, add executable flag only if
    // we are going the program header is loadable.
    // NOTE: Adding exec to other sections will lead to segmentation
    //       fault
    for ( int i=0; i<header.ProgramHeaders(); i++ ) {
        RawProgramHeader segment;

        progReader >> (Elf64_Phdr&) segment;

        if (segment.IsLoadableSegment())  
            segment.AddExecutable();

        progWriter << (Elf64_Phdr&) segment; 
    }

    fileWriter.Flush();
    stdFile.close();

    return 0;
}
