/*
 * Valgrind is flagging up a bug where some of the data in the final
 * file returned by ElfFile is unintialised.
 *
 * Its not clean where this memorry is being set, but it could be an
 * alignment gap in one of the objects.
 *
 * The idea is to use to template magic to copy every so slightly more 
 * of the file, until valgrind hits the bug - telling us exactly where the 
 * memorry is.
 *
 */
#include "elfParser.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"
#include "dataLump.h"
#include "util_time.h"
#include "defer.h"

ElfParser *p;
DataLump<5000> outfile;
ElfHeaderX86_64 *header;
Elf64_Ehdr *hdr;

using namespace std;

template<long SIZE>
int Write() 
{
    OFStreamWriter output("out.o");
    BinaryReader readPos (outfile);
    readPos.Begin().Read(BinaryWriter(output),SIZE);
    output.Flush();
    output.close();
    return Write<SIZE+1>();
}

template<>
int Write<300>()
{
    return 0;
}

/*
 * Validate a basic object file
 */
int main(int argc, const char *argv[])
{
    ElfFileReader f("../elf2elf/isYes/isYes.o");
    
    p = new ElfParser(f);
    
    //parse the input file ElfContent content = p->Content();
    ElfContent content = p->Content();
    ElfFile file( content);
    
    // Write out the data
    file.WriteToFile(outfile);

	delete header;
	delete p;

    OFStreamWriter output("out.o");
    BinaryReader readPos (outfile);
    readPos.Begin().Read(BinaryWriter(output),outfile.Size());
    output.Flush();
    output.close();

    // If valgrind returns an error, enable this function call to find 
    // the location of the problem:
    // Write<1>();

    return 0;
}
