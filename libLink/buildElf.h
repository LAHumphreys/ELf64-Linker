#ifndef BUILD_ELF_H
#define BUILD_ELF_H

#include "binaryReader.h"
#include "dataVector.h"

struct ElfContent {
    const ElfHeaderX86_64 &header;
    std::vector<Section *>& sections;
    std::vector<ProgramHeader *>& progHeaders;
    std::vector<Symbol *>& symbols;
};

class ElfFile{
public:
    ElfFile(ElfContent &data);
    void MakeNewHeader(ElfContent &data);

private:
    DataVector file;
    DataVector sectionData;
    ElfHeaderX86_64 header;
};


/*
 * We're trying to build this:
 *
 *  +-------------------------------+
 *  | ELF File Header               |
 *  +-------------------------------+
 *  | Program Header for segment #1 |
 *  +-------------------------------+
 *  | Program Header for segment #2 |
 *  +-------------------------------+
 *  | ...                           |
 *  +-------------------------------+
 *  | Contents (Byte Stream)        |
 *  | ...                           |
 *  +-------------------------------+
 *  | Section Header for section #1 |
 *  +-------------------------------+
 *  | Section Header for section #2 |
 *  +-------------------------------+
 *  | ...                           |
 *  +-------------------------------+
 *  | ".shstrtab" section           |
 *  +-------------------------------+
 *  | ".symtab"   section           |
 *  +-------------------------------+
 *  | ".strtab"   section           |
 *  +-------------------------------+
 *  
 */

/*
 * This class will format the data up, but it is not expected to
 * perform any non trivial relocation etc.
 *
 * +------------------------+--------------------+
 * |                 ELF File Header:            |
 * +------------------------+--------------------+
 * | Callers Responsibility | Our Responsibility |
 * +------------------------+--------------------+
 * | e_type                 | e_phoff            |
 * +------------------------+--------------------+
 * | e_machine              | e_shoff            |
 * +------------------------+--------------------+
 * | e_version              | e_phnum            |
 * +------------------------+--------------------+
 * | e_entry                | e_shnum            |
 * +------------------------+--------------------+
 * | e_flags                | e_shstrndx         |
 * +------------------------+--------------------+
 * | e_ehsize               |                    |
 * +------------------------+--------------------+
 * | e_phentsize            |                    |
 * +------------------------+--------------------+
 * | e_shentsize            |                    |
 * +------------------------+--------------------+
 *
 */
#endif
