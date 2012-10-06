using namespace std;



#include <iostream>
#include <sstream>

template<>
ElfParser<ElfFileReader>::ElfParser(const string &fname) 
  : reader(fname),
    stringTable(reader, 0 ),
    headerStrings(reader, 0)

{
    // if we try to index with these before they are set we want an error to
    // happen
    symidx=-1;
    stridx=-1;

    // Properties
    this->filename = fname;

    // Load the header from file
    const BinaryPosition& r = reader.Begin();
    header = new ElfHeaderX86_64(r);

    ReadSections();
    ReadProgramHeaders();

    ReadSymbols();

}

template<class T>
void ElfParser<T>::ReadSections() {
    // Declare an array to hold the sections
    sections.resize(header->Sections());

    // Read in the section constants
    long hdrSize = header->SectionHeaderSize();
    Pos tableStart = reader.Begin() + header->SectionTableStart();

    // Which section holds the (header) string table?
    long sidx = header->StringTableIndex();

    // We need the address of the string table
    Elf64_Shdr stableHeader;
    Pos stringTablePos = tableStart + sidx*hdrSize;
    stringTablePos.Read(&stableHeader, hdrSize);

    headerStrings =  stableHeader.sh_offset;
    Pos readPos = tableStart;

    for ( int i=0; i<header->Sections(); ++i) {
       sections[i] = new Section(readPos, headerStrings);
       if (sections[i]->Name() == ".symtab" ) symidx = i;
       if (sections[i]->Name() == ".strtab" ) stridx = i;
       readPos += hdrSize;
    }
    stringTable = sections[stridx]->DataStart();
}

template<class T>
void ElfParser<T>::ReadProgramHeaders() {
    // Declare an array to hold the sections
    progHeaders.resize(header->ProgramHeaders());

    // Read in the section constants
    Pos readPos = reader.Begin() + header->ProgramHeadersStart();
    long hdrSize =  header->ProgramHeaderSize();

    for ( int i=0; i<header->ProgramHeaders(); ++i) {
       progHeaders[i] = new ProgramHeader( readPos, sections);
       readPos += hdrSize;
    }
}

template<class T>
void ElfParser<T>::ReadSymbols() {
    Section * symTable = sections[symidx];
    symbols.resize(symTable->NumItems());

    Pos readPos = reader.Begin() + symTable->DataStart();

    for ( int i=0; i < symTable->NumItems(); ++i) {
        symbols[i] = new Symbol(readPos,stringTable);
        readPos += symTable->ItemSize();
    }
}

template<class T>
ElfParser<T>::~ElfParser () {
    delete header;
    for ( auto ptr : sections ) delete ptr;
    for ( auto ptr : symbols ) delete ptr;
    for ( auto ptr : progHeaders ) delete ptr;
}

template<class T>
string ElfParser<T>::PrintLink() {
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

