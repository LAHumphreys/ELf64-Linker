#ifndef SECTIONH_EADER
#define SECTIONH_EADER
#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
#include "binaryData.h"

class SectionHeader: public Elf64_Shdr {
public:
    SectionHeader(const BinaryReader& loc); /* IO interface construction */
    SectionHeader(const Elf64_Shdr& rhs);  /* copy construction */
    SectionHeader(); /* produces a null object */

    // properties
    inline bool Writeable() { return sh_flags & SHF_WRITE; }
    inline bool Allocate() { return sh_flags &  SHF_ALLOC; }
    inline bool Executable(){ return sh_flags & SHF_EXECINSTR; }
    inline bool IsSymTable() { return sh_type ==  SHT_SYMTAB; }
    inline bool IsStringTable() {return sh_type ==  SHT_STRTAB; }
    inline bool IsRelocTable() {return sh_type ==  SHT_RELA; }
    inline Elf64_Xword& Alignment() { return sh_addralign; }
    
    // Data properties
    inline Elf64_Off& DataStart() { return sh_offset; }
    inline Elf64_Xword& DataSize() { return sh_size; }
    inline Elf64_Xword& ItemSize() { return sh_entsize; }
    inline Elf64_Word& NameOffset() { return sh_name; }

    /*address in memory*/
    inline Elf64_Addr& Address() { return sh_addr; } 



    // Calculated Properties
    inline Elf64_Xword NumItems() { return DataSize() / ItemSize(); }
    inline Elf64_Off DataEnd() { return DataStart() + DataSize(); }
    inline bool HasFileData() { return sh_type != SHT_NOBITS; }

    virtual inline size_t Size() {return sizeof(Elf64_Shdr);}
    virtual ~SectionHeader() {}

protected:
    Elf64_Xword& RawFlags() {return sh_flags;}
    Elf64_Word& RawType() {return sh_type;}
    Elf64_Word& RawLink() {return sh_link;}
    Elf64_Word& RawInfo() {return sh_info;}

    
};
#endif
