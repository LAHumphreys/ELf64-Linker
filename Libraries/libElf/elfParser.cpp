using namespace std;

#include <iostream>
#include <sstream>

#include "elfParser.h"
#include <memory>
#include "logger.h"

ElfParser::ElfParser(const FileLikeReader &f): 
   reader(f), stringTable(reader), headerStrings(reader)

{
    linkSections=0;
    // if we try to index with these before they are set we want an
    // error to happen
    symidx=-1;
    stridx=-1;

    header = new ElfHeaderX86_64(reader.Begin());

    ReadSections();
    ReadProgramHeaders();

    ReadSymbols();

    WriteStringTable();
    UpdateSymbolTable();

}

void ElfParser::ReadSections() {
    // Declare an array to hold the sections
    sections.resize(header->Sections());

    // Read in the section constants
    long hdrSize = header->SectionHeaderSize();
    BinaryReader tableStart = reader.Begin().operator+
                            (header->SectionTableStart());

    // Which section holds the (header) string table?
    long sidx = header->StringTableIndex();

    // We need the address of the string table
    Elf64_Shdr stableHeader;
    (tableStart + sidx*hdrSize).Read(&stableHeader, hdrSize);

    headerStrings =  stableHeader.sh_offset;
    BinaryReader nextSection = tableStart;

    for ( int i=0; i<header->Sections(); ++i) {
       sections[i] = new Section(nextSection, headerStrings);
       if (sections[i]->Name() == ".symtab" ) symidx = i;
       if (sections[i]->Name() == ".strtab" ) stridx = i;
       if (sections[i]->IsLInkSection() ) ++linkSections;
       sectionMap[sections[i]->Name()] = i;
       nextSection += hdrSize;

       LOG_FROM ( 
            LOG_VERY_VERBOSE, 
            "ElfParser::ReadSections", 
              "Read a new section:" 
             + sections[i]->Name() 
             + "\n" + sections[i]->Descripe()
       )
    }
    stringTable = sections[stridx]->DataStart();
}

void ElfParser::ReadProgramHeaders() {
    // Declare an array to hold the sections
    progHeaders.resize(header->ProgramHeaders());

    // Read in the section constants
    BinaryReader readPos =   reader.Begin() 
                           + header->ProgramHeadersStart();

    for ( int i=0; i<header->ProgramHeaders(); ++i) {
       progHeaders[i] = new ProgramHeader( readPos, sections);
    }
}

void ElfParser::ReadSymbols() {
    Section * symTable = sections[symidx];
    symbols.resize(symTable->NumItems());

    BinaryReader readPos = reader.Begin() + 
                             symTable->DataStart();

for ( size_t i=0; i < symTable->NumItems(); ++i) {
        symbols[i] = new Symbol(readPos,stringTable);
        if ( symbols[i]->IsLinkSymbol() ) ++linkSymbols;
        symbolMap[symbols[i]->Name()] = i;
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

    link << "# Header: nsegs, nsyms, nrels flags" << endl;
    link << LinkSections() << " " << LinkSymbols() << " ";
    link << RelocCount() << " " << header->LinkFlags() << endl;

    link << "# Section headers" << endl;
    for ( auto section: sections ) {
        if ( section->IsLInkSection() )
            link << section->WriteLinkHeader() << endl;
    }

    link << "# Symbol Table" << endl;
    for ( auto symbol: symbols ) {
        if ( symbol->IsLinkSymbol() ) 
            link << symbol->LinkFormat() << endl;
    }

    link << "# section data" << endl;
    for ( auto section: sections ) {
        if ( section->IsLInkSection() )
            link << section->WriteLinkData() << endl;
    }
    return link.str();
}

ElfContent ElfParser::Content() {
    ElfContent content = {
        *(this->header),
        this->sections,
        this->progHeaders,
        this->symbols,
        this->sectionMap,
        this->symbolMap

    };
    return content;
}

void ElfParser::WriteStringTable () {
    for ( Section* sec : sections ) {
        if ( sec->Name().length() > 0 )
            sec->NameOffset() = sh_strtab.AddString(sec->Name().c_str());
        else
            sec->NameOffset() = 0;
    }
    Section* shtab = Section::MakeNewStringTable(sh_strtab, &sh_strtab, ".shstrtab");
    // Swap in the new string table
    delete sections[sectionMap[".shstrtab"]];
    sections[sectionMap[".shstrtab"]] = shtab;
}

void ElfParser::UpdateSymbolTable() {
    Section& symTable = *sections[sectionMap[".symtab"]];

    if ( symbols.size() == 0 ) {
        symTable.DataSize() = 0;
    } else {
        symTable.DataSize() = symbols.size() * symTable.ItemSize();
    }
    // Now write the new data
    shared_ptr<Data> data = symTable.GetData();
    data->Resize(symTable.DataSize());
    BinaryWriter w = data->Writer();
    for ( size_t i=0; i<symTable.NumItems(); ++i) {
        w << symbols[i]->RawItem();
    }
}
