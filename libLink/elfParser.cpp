#ifndef ElfParser_H
   #include "elfParser.h"
#endif
#include <iostream>
using namespace std;

ElfParser::ElfParser(const string &fname) : reader(fname) {
    header = new ElfHeaderX86_64(reader,0);
    sections.resize(header->Sections());
    long stringTable;

    // Read in the section constants
    int readAddr = header->SectionTableStart();
    int hdrSize = header->SectionHeaderSize();

    // Which section holds the string table?
    int sidx = header->StringTableIndex();

    // We need the address of the string table
    Elf64_Shdr stableHeader;
    reader.Read(sidx*hdrSize +readAddr, &stableHeader, 
                                         hdrSize);
    stringTable = stableHeader.sh_offset;

    for ( int i=0; i<header->Sections(); ++i) {
        sections[i] = new Section(reader, readAddr,
                                          stringTable);
        readAddr += hdrSize;
    }
}
ElfParser::~ElfParser () {
    delete header;
}
