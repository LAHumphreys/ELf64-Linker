#include "elfReader.h"
#include "elfHeader.h"
#include <vector>
#include "symbol.h"
#include "programHeader.h"
#include "reader.h"
#include <forward_list>
#include "reloc.h"
#include "binaryReader.h"
#include "buildElf.h"

/**
    \class   ElfParser
    \brief   Read a valid ELF file into friendly libLink objects
    \details  Description of the class
*/
class ElfParser {
public:
    // C'tor / D'tor
    ElfParser (const FileLikeReader &f);
    virtual ~ElfParser ();

    // Create a string representing the object file in LINK format
    string PrintLink();

    // Properties
    string FileName() { return filename; }
    int SegmentCount() { return sections.size(); }
    int LinkSections() { return linkSections; }
    int SymbolCount() { return sections.size(); }
    int LinkSymbols() { return linkSymbols; }
    int RelocCount() { return 0; /*not yet implemented */}

    ElfContent Content();
protected:
    void ReadSymbols();
    void ReadProgramHeaders();
    void ReadSections();

private:
    BinaryReader reader;
    BinaryReader stringTable;
    BinaryReader headerStrings;

    /* data */
    ElfHeaderX86_64 *header;
    std::vector<Section *> sections;
    std::vector<ProgramHeader *> progHeaders;
    std::vector<Symbol *> symbols;
    std::map<string,int> sectionMap;

    // We're want to rapidly add things, and only ever want to
    // iterate through. 
    std::forward_list<Relocation *> relocations;
    int symidx;
    int stridx;
    int linkSections;
    int linkSymbols;
    string filename;
};

