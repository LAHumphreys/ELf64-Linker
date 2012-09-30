#include "elfReader.h"
#include "elfHeader.h"
#include <vector>
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
    virtual ~ElfParser ();

private:
    ElfReader reader;
    ElfHeaderX86_64 *header;
    /* data */
    std::vector<Section *> sections;
};
