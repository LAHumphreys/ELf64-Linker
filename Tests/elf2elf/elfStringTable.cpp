#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"
#include <elf.h>
#include "dataLump.h"

DataLump<5000> outfile;
SectionHeader* stringheaders;
Elf64_Shdr* sections;
Elf64_Shdr* stringTableHeader;

int ValidHeader(stringstream& log );

int main(int argc, const char *argv[])
{
    /*
     * Tests to validate the string table written by ElfFile
     */
    ElfFileReader f("../elf2elf/isYes/isYes.o");
    
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();
    ElfFile file( content);
    
    // Write out the data
    file.WriteToFile(outfile.Writer());

    // read in the section data
    BinaryReader readPos(outfile);
    ElfHeaderX86_64 header(readPos);
    sections = new Elf64_Shdr[header.Sections()];

    readPos = header.SectionTableStart();
    for ( int i=0; i< header.Sections(); i++ ) {
	readPos >> sections[i];
    }

    stringTableHeader = &(sections[header.StringTableIndex()]);

    Test("Header format",(loggedTest)ValidHeader).RunTest();

    return 0;
}

int ValidHeader(stringstream& log ) {
    int retCode = 0;

    if ( stringTableHeader->sh_type != SHT_STRTAB ) {
        log << stringTableHeader->sh_type << " , " << SHT_STRTAB << endl;
        log << "Invalid section type" << endl;
	log << "Header Block: " << endl;
	log << "Flags: " << stringTableHeader->sh_flags << endl;
	log << "Type: " << stringTableHeader->sh_type << endl;
	log << "AddrAlign: " << stringTableHeader->sh_addralign << endl;
	log << "Offset: " << stringTableHeader->sh_offset << endl;
	log << "Size: " << stringTableHeader->sh_size << endl;
	log << "Entry Size: " << stringTableHeader->sh_entsize << endl;
	log << "Name Addr: " << stringTableHeader->sh_name << endl;
	log << "Addr: " << stringTableHeader->sh_addr << endl;
        retCode = 1;
    }

    return retCode;
}
