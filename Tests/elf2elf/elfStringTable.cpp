#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"
#include <elf.h>
#include "dataLump.h"
#include "defer.h"

DataLump<5000> outfile;
SectionHeader* stringheaders;
Elf64_Shdr* sections;
SectionHeader* stringTableHeader;
std::map<string, int>* sectionMap;

int ValidHeader(stringstream& log );
int SectionNames(stringstream& log );

int main(int argc, const char *argv[])
{
    /*
     * Tests to validate the string table written by ElfFile
     */
    ElfFileReader f("../elf2elf/isYes/isYes.o");
    
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();
    sectionMap = &content.sectionMap;

    ElfFile file( content);
    
    // Write out the data
    outfile.Fill(0,'\0',5000);
    file.WriteToFile(outfile.Writer());

    // read in the section data
    BinaryReader readPos(outfile);
    ElfHeaderX86_64 header(readPos);
    sections = new Elf64_Shdr[header.Sections()];
    DEFER ( delete [] sections; ) 

    readPos = header.SectionTableStart();

    for ( int i=0; i< header.Sections(); i++ ) {
        readPos >> sections[i];
    }

    stringTableHeader = new SectionHeader(sections[header.StringTableIndex()]);

    Test("Header format",(loggedTest)ValidHeader).RunTest();
    Test("Header format",(loggedTest)SectionNames).RunTest();


    return 0;
}

int ValidHeader(stringstream& log ) {
    int retCode = 0;

    if ( stringTableHeader->sh_type != SHT_STRTAB ) {
        log << stringTableHeader->sh_type << " , " << SHT_STRTAB << endl;
        log << "Invalid section type" << endl;
        log << "Header Block: " << endl;
        log << stringTableHeader->Descripe() << endl;
        retCode = 1;
    }

    return retCode;
}

int SectionNames(stringstream& log ) {
    for ( auto& pair: *sectionMap) {
        // get the first and second elements
        string originalName = pair.first;
        SectionHeader newHdr(sections[pair.second]);
        BinaryReader strTable(outfile,newHdr.NameOffset());
        string newName = strTable.ReadString();
        cout << originalName << " -> " << newName << endl;
    }
}
