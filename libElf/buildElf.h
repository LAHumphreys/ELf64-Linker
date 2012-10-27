#ifndef BUILD_ELF_H
#define BUILD_ELF_H

#include "binaryReader.h"
#include "binaryWriter.h"
#include "dataVector.h"
#include <vector>
#include <map>
#include <algorithm>
#include "elf.h"
#include "elfHeader.h"
#include "programHeader.h"
#include "section.h"
#include "symbol.h"

struct ElfContent {
    ElfHeaderX86_64 &header;
    std::vector<Section *>& sections;
    std::vector<ProgramHeader *>& progHeaders;
    std::vector<Symbol *>& symbols;
    std::map<string, int>& sectionMap;
};

class ElfFile{
public:
    ElfFile(ElfContent &data);
    void MakeNewHeader(ElfContent &data);
    BinaryWriter ProcessProgHeaders(ElfContent& data);
    BinaryWriter WriteDataSections( ElfContent& data, 
                                    ProgramHeader& prog, 
                                    BinaryWriter& writer);
    void WriteSectionHeaders(ElfContent& data);
protected:
    void InitialiseFile(ElfContent& data);
    void InitialiseHeader(ElfContent& data);
    bool IsSpecialSection(Section& s);
    template<class T>
    void SortByAddress(T start, T end);

private:
    Elf64_Ehdr header;

    //final data
    DataVector file;

    // utility data
    long sectionDataLength;
    BinaryWriter programHeadersStart;
    BinaryWriter dataSectionStart;
    BinaryWriter sectionHeadersStart;
};

template<class T> 
void ElfFile::SortByAddress(T start, T end) {
    typedef typename std::iterator_traits<T>::value_type item;
    auto lt = [] (item lhs, item rhs) -> bool 
                 {
                     return lhs->Address() < rhs->Address();
                 };
    sort(start,end,lt);
}



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
 *                The Elf Header
 *              +----------------+
 * This class will format the data up, but it is not expected to
 * perform any non trivial relocation etc.
 *
 * +------------------------+--------------------+
 * |                 ELF File Header             |
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

/* 
 *            Program headers and Section headers
 *          +-------------------------------------+
 *
 * Program headers will be written in the order in which they appear
 * in the program headers array. 
 *
 * In the absence of program headers, section headers (and their 
 * data will be written in the order in which they appear in the
 * input array, although the final 3 will always be, if included 
 * in the input, .shstrtab, then .symtab and finally .strtab.
 *   If program headers are provided, then sections are written as 
 * they are mentioned by the program headers, with section that 
 * define a v_addr being written at the correct relative address 
 * to the start of the program header. It is the callers 
 * responsibility to ensure this isn't a stupid memorry location. 
 *
 * +------------------------+--------------------+
 * |              ELF Program Header             |
 * +------------------------+--------------------+
 * | Callers Responsibility | Our Responsibility |
 * +------------------------+--------------------+
 * | p_type                 | p_offset           |
 * +------------------------+--------------------+
 * | p_flags                | p_filesz           |
 * +------------------------+--------------------+
 * | p_vaddr                |                    |
 * +------------------------+--------------------+
 * | p_paddr                |                    |
 * +------------------------+--------------------+
 * | p_memsz                |                    |
 * +------------------------+--------------------+
 * | p_align                |                    |
 * +------------------------+--------------------+
 *
 * +------------------------+--------------------+
 * |              ELF Section Header             |
 * +------------------------+--------------------+
 * | Callers Responsibility | Our Responsibility |
 * +------------------------+--------------------+
 * | sh_name                | sh_offset          |
 * +------------------------+--------------------+
 * | sh_type                | sh_link(=0)        |
 * +------------------------+--------------------+
 * | sh_flags               |                    |
 * +------------------------+--------------------+
 * | sh_addr                |                    |
 * +------------------------+--------------------+
 * | sh_size                |                    |
 * +------------------------+--------------------+
 * | sh_info                |                    |
 * +------------------------+--------------------+
 * | sh_addralign           |                    |
 * +------------------------+--------------------+
 * | sh_entsize             |                    |
 * +------------------------+--------------------+
 *
 *
 *  Aligning Data
 * +-------------+
 * -> Loadable segments must be aligned to p_align value in the 
 *    header file section: p_vaddr % p_align = p_offset % p_align. 
 *    This will be handled here
 * -> Now it is required that: sh_addr % sh_addralign = 0. However 
 *    the caller must handle this, as changing anything here would 
 *    require re-locating symbols
 */
#endif
