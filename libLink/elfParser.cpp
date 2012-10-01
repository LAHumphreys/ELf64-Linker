#ifndef ElfParser_H
   #include "elfParser.h"
#endif
using namespace std;



#include <iostream>

ElfParser::ElfParser(const string &fname) : reader(fname) {

    // if we try to index with these before they are set we want an error to
    // happen
    symidx=-1;
    stridx=-1;

    // Load the header from file
    header = new ElfHeaderX86_64(reader,0);

    ReadSections();

    ReadSymbols();

}

void ElfParser::ReadSections() {
    // Declare an array to hold the sections
    sections.resize(header->Sections());

    // Read in the section constants
    long readAddr = header->SectionTableStart();
    long hdrSize = header->SectionHeaderSize();

    // Which section holds the string table?
    long sidx = header->StringTableIndex();

    // We need the address of the string table
    Elf64_Shdr stableHeader;
    reader.Read(sidx*hdrSize +readAddr, &stableHeader, 
                                         hdrSize);
    long headerStringTable = stableHeader.sh_offset;

    for ( int i=0; i<header->Sections(); ++i) {
       sections[i] = new Section(reader, readAddr,
                                         headerStringTable);
       if (sections[i]->Name() == ".symtab" ) symidx = i;
       if (sections[i]->Name() == ".strtab" ) stridx = i;
       readAddr += hdrSize;
    }
    stringTable = sections[stridx]->DataStart();
}

ElfParser::~ElfParser () {
    delete header;
    for ( auto ptr : sections ) delete ptr;
    for ( auto ptr : symbols ) delete ptr;
}

void ElfParser::ReadSymbols() {
    Section * symTable = sections[symidx];
    symbols.resize(symTable->NumItems());
    long readAddr = symTable->DataStart();
    for ( int i=0; i < symTable->NumItems(); ++i) {
        symbols[i] = new Symbol(reader,readAddr,stringTable);
        readAddr += symTable->ItemSize();
    }
}
