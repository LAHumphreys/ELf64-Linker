#ifndef RELOCATION_H
   #define RELOCATION_H
#include "flags.h"
#include "elf.h"
class ElfReader;

class Relocation {
public:
    Relocation (ElfReader &reader, long offset, string section);

    // atributes
    string& Section() { return section; }
    

    // Calculated properties
    size_t Size() { return sizeof(Elf64_Rel); }
    string LinkFormat();
    Elf64_Xword SymbolIndex() { 
        return ELF64_R_SYM(reloc.r_info);
    }
    Elf64_Xword ElfRelocType() { 
        return ELF64_R_TYPE(reloc.r_info);
    }

private:
    // helper functions
    void ConfigureFlags();
    void ConvertFromElf();
    // data
    string section;
    Elf64_Rel reloc;
    Flags type;
    string x86_type;
    short size;
};
#endif
