#include "elfReader.h"
#include "elfHeader.h"
#include <vector>
#include "symbol.h"
#include "programHeader.h"
#include "reader.h"
#include <forward_list>
#include "reloc.h"

/**
    \class   ElfParser
    \brief   Read a valid ELF file into friendly libLink objects
    \details  Description of the class
*/
template< class ReaderType>
class ElfParser {
public:
    // C'tor / D'tor
    ElfParser (const string &fname);
    virtual ~ElfParser ();

    // Create a string representing the object file in LINK format
    string PrintLink();

    // Properties
    string FileName() { return filename; }
    int SegmentCount() { return sections.size(); }
    int SymbolCount() { return sections.size(); }
    int RelocCount() { return 0; /*not yet implemented */}
protected:
    void ReadSymbols();
    void ReadProgramHeaders();
    void ReadSections();

private:
    typedef SimpleBinaryPosition Pos;
    ReaderType reader;

    Pos stringTable;
    Pos headerStrings;

    /* data */
    ElfHeaderX86_64 *header;
    std::vector<Section *> sections;
    std::vector<ProgramHeader *> progHeaders;
    std::vector<Symbol *> symbols;
    // We're want to rapidly add things, and only ever want to
    // iterate through. 
    std::forward_list<Relocation *> relocations;
    int symidx;
    int stridx;
    string filename;
};

#include "elfParser.hpp"
