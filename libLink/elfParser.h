#include "elfReader.h"
#include "elfHeader.h"
#include <vector>
#include "symbol.h"
#ifndef ElfParser_H
   #define ElfParser_H
#endif
/**
    \class   ElfParser
    \brief   Read a valid ELF file into friendly libLink objects
    \details  Description of the class
*/
class ElfParser {
public:
    ElfParser (const string &fname);
    void ReadSymbols();
    virtual ~ElfParser ();

private:
    long stringTable;
    ElfReader reader;
    ElfHeaderX86_64 *header;
    /* data */
    std::vector<Section *> sections;
    std::vector<Symbol *> symbols;
    int symidx;
    int stridx;
};
