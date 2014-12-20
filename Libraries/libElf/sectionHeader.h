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
    inline bool Writeable() const { return sh_flags & SHF_WRITE; }
    inline bool Allocate() const { return sh_flags &  SHF_ALLOC; }
    inline bool Executable()const { return sh_flags & SHF_EXECINSTR; }
    inline bool IsSymTable() const { return sh_type ==  SHT_SYMTAB; }
    inline bool IsStringTable() const {return sh_type ==  SHT_STRTAB; }
    inline bool IsRelocTable() const {return sh_type ==  SHT_RELA; }
    inline bool IsNull() const { return sh_type == SHT_NULL; }
    inline Elf64_Xword& Alignment() { return sh_addralign; }
    
    // Data properties
    inline Elf64_Off& DataStart() { return sh_offset; }
    inline Elf64_Xword& DataSize() { return sh_size; }
    inline Elf64_Xword& ItemSize() { return sh_entsize; }
    inline Elf64_Word& NameOffset() { return sh_name; }

    // Data property accessors
    inline const Elf64_Off& DataStart() const { return sh_offset; }
    inline const Elf64_Xword& DataSize() const { return sh_size; }
    inline const Elf64_Xword& ItemSize() const { return sh_entsize; }
    inline const Elf64_Word& NameOffset() const { return sh_name; }
    inline const Elf64_Xword& Alignment() const { return sh_addralign; }

    /*address in memory*/
    inline const Elf64_Addr& Address() const { return sh_addr; } 
    inline Elf64_Addr& Address() { return sh_addr; } 

    string Descripe() const;



    // Calculated Properties
    inline Elf64_Xword NumItems() const { 
        return DataSize() / ItemSize(); 
    }
    inline Elf64_Off DataEnd() const { 
        return DataStart() + DataSize(); 
    }
    inline Elf64_Addr AddrEnd() const {
    	return Address() + DataSize();
    }
    inline bool HasFileData() const { 
        return sh_type != SHT_NOBITS; 
    }

    virtual inline size_t Size() const {
        return sizeof(Elf64_Shdr);
    }

    virtual ~SectionHeader() {}

protected:
    Elf64_Xword& RawFlags() {return sh_flags;}
    Elf64_Word& RawType() {return sh_type;}
    Elf64_Word& RawLink() {return sh_link;}
    Elf64_Word& RawInfo() {return sh_info;}

    
};
#endif
