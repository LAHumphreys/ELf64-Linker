#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
#include "elfReader.h"
#include "data.h"
using namespace std;
#ifndef SectionX86_64
   #define SectionX86_64

class StringTable;

class Section {
    friend class X86Parser;
public:
    Section (ElfReader &reader, long offset, long stable);
    Section (string header, StringTable* );
    ~Section ();
    Elf64_Xword GetFlags();

    // Methods
    string GetLinkFlags();
    string WriteLinkHeader();
    string WriteLinkData();

    // attributes
    size_t Size() {return sizeof(Elf64_Shdr);}
    Elf64_Addr& Address() { return elfHeader.sh_addr; }

    // properties
    bool Writeable() { return elfHeader.sh_flags & SHF_WRITE; }
    bool Allocate() { return elfHeader.sh_flags &  SHF_ALLOC; }
    bool Executable(){ return elfHeader.sh_flags & SHF_EXECINSTR; }
    bool IsSymTable() { return elfHeader.sh_type ==  SHT_SYMTAB; }
    string Name() { return name; }
    Elf64_Xword& Alignment() { return elfHeader.sh_addralign; }


    // Data
    Elf64_Off& DataStart() { return elfHeader.sh_offset; }
    uint64_t& DataSize() { return elfHeader.sh_size; }
    uint64_t& ItemSize() { return elfHeader.sh_entsize; }

    // Calculated Properties
    uint64_t NumItems() { return DataSize() / ItemSize(); }
    Elf64_Off DataEnd() { return DataStart() + DataSize(); }
    bool HasFileData() { 
        return elfHeader.sh_type != SHT_NOBITS; 
    }



private:
    Data * data;
    Elf64_Shdr elfHeader;
    StringTable *stringTable;
    string name;
    Flags sh_flags;
    /* data */
};

#endif
