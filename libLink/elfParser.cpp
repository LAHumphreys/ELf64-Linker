#ifndef ElfParser_H
   #include "elfParser.h"
#endif
using namespace std;



#include <iostream>
#include <sstream>

ElfParser::ElfParser(const string &fname) : reader(fname) {

    // if we try to index with these before they are set we want an error to
    // happen
    symidx=-1;
    stridx=-1;

    // Properties
    this->filename = fname;

    // Load the header from file
    header = new ElfHeaderX86_64(reader,0);

    ReadSections();
    ReadProgramHeaders();

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
    headerStringTable = stableHeader.sh_offset;

    for ( int i=0; i<header->Sections(); ++i) {
       sections[i] = new Section(reader, readAddr,
                                         headerStringTable);
       if (sections[i]->Name() == ".symtab" ) symidx = i;
       if (sections[i]->Name() == ".strtab" ) stridx = i;
       readAddr += hdrSize;
    }
    stringTable = sections[stridx]->DataStart();
}

void ElfParser::ReadProgramHeaders() {
    // Declare an array to hold the sections
    progHeaders.resize(header->ProgramHeaders());

    // Read in the section constants
    long readAddr = header->ProgramHeadersStart();
    long hdrSize = header->ProgramHeaderSize();

    for ( int i=0; i<header->ProgramHeaders(); ++i) {
       progHeaders[i] = new ProgramHeader( reader, 
                                           readAddr,
                                           sections);
       readAddr += hdrSize;
       cout << progHeaders[i]->WriteLink() << endl;
    }
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

ElfParser::~ElfParser () {
    delete header;
    for ( auto ptr : sections ) delete ptr;
    for ( auto ptr : symbols ) delete ptr;
    for ( auto ptr : progHeaders ) delete ptr;
}

string ElfParser::PrintLink() {
    ostringstream link;
    link << "# LINK formated file created from " << FileName();
    link << " by elf2link" << endl;
    link << "LINK" << endl;

    link << "# Header: nsegs, nsyms, nrels" << endl;
    link << SegmentCount() << " " << SymbolCount() << " ";
    link << RelocCount() << endl;

    link << "# Section headers" << endl;
    for ( auto section: sections ) {
        link << section->WriteLinkHeader() << endl;;
    }

    link << "# Symbol Table" << endl;
    for ( auto symbol: symbols ) {
        link << symbol->LinkFormat() << endl;
    }

    link << "# section data" << endl;
    for ( auto section: sections ) {
        link << section->WriteLinkData() << endl;
    }
    return link.str();
}

