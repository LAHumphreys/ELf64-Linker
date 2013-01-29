#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"
#include <elf.h>
#include "dataLump.h"

ElfParser *p;
DataLump<5000> outfile;
Section* stringheader;

int ValidHeader(stringstream& log );

int main(int argc, const char *argv[])
{
    /*
     * Tests to validate the string table written by ElfFile
     */
    ElfFileReader f("../elf2elf/isYes/isYes.o");
    
    p = new ElfParser(f);
    
    //parse the input file
    ElfContent content = p->Content();
    ElfFile file( content);
    
    // Write out the data
    file.WriteToFile(outfile.Writer());
    Test("Header format",(loggedTest)ValidHeader).RunTest();
    delete p;

    return 0;
}

int ValidHeader(stringstream& log ) {
    int retCode = 0;
    BinaryReader readPos(outfile);
    ElfHeaderX86_64 header(readPos);
    readPos += header.SectionTableStart() 
             + header.StringTableIndex() * header.SectionHeaderSize();
    stringheader = (Elf64_Shdr) readPos.Dup(header.SectionHeaderSize());

    if ( stringheader->sh_type != SHT_STRTAB ) {
        log << stringheader->sh_type << " , " << SHT_STRTAB << endl;
        log << "Invalid section type" << endl;
        retCode = 1;
    }

    if ( retCode == 0 ) {
    }
    delete stringheader;

    return retCode;
}
