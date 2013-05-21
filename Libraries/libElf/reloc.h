#ifndef RELOCATION_H
   #define RELOCATION_H
#include "flags.h"
#include "elf.h"
class BinaryReader;

class Relocation {
public:
    Relocation ( const BinaryReader& reader, 
                 const string section);

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

protected:
    static const Flags& TypeFlags();

    static Flags::Mask Flags_SHF_WRITE;
    static Flags::Mask Flags_SHF_ALLOC;
    static Flags::Mask Flags_SHF_EXECINSTR;
    static Flags::Mask Flags_Absolute;
    static Flags::Mask Flags_Relative;
    static Flags::Mask Flags_Symbol;
    static Flags::Mask Flags_1Byte;
    static Flags::Mask Flags_2Byte;
    static Flags::Mask Flags_4Byte;
    static Flags::Mask Flags_8Byte;
    static Flags::Mask Flags_HasAddendum;
    static Flags::Mask Flags_ZeroExtended;
    static Flags::Mask Flags_SignExtended;

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
