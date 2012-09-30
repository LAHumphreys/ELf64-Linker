#ifndef SectionX86_64
   #include "section.h"
#endif
#include <sstream>
#include "stringTable.h"

Section::Section(string header, StringTable * stable)
    : sh_flags("") {
    sh_flags.AddFlag('W', "SHF_WRITE");
    sh_flags.AddFlag('A', "SHF_ALLOC");
    sh_flags.AddFlag('C', "SHF_EXECINSTR");
    this->stringTable = stable;

    // Build the flags
    stringstream s(header);
    string name;
    string flags;
    Elf64_Addr addr;
    Elf64_Xword size;
    Elf64_Xword align;

    s >> name;
    s >> hex >> addr;
    s >> hex >> size;
    s >> flags;
    sh_flags.SetFlags(flags);
    s >> align;

    elfHeader.sh_name = stringTable->AddString(name.c_str());
    elfHeader.sh_type = SHT_PROGBITS; // sym tables etc may override
    elfHeader.sh_flags = GetFlags();
    elfHeader.sh_addr = addr;
    elfHeader.sh_offset = 0; // we don't know this yet
    elfHeader.sh_size = size;
    elfHeader.sh_addralign = align;
    // only used for specialisations with fixed sized entries
    // (tables)
    elfHeader.sh_entsize = 0; 
    
    // These don't appear to be used
    elfHeader.sh_link = 0;
    elfHeader.sh_info = 0;
}


Elf64_Xword Section::GetFlags() {
    // As it happens, the internal mask of our Flags object is
    // exactly this, but this safe guards against a change in elf.h
    Elf64_Xword result = 0;
    if ( sh_flags["SHF_WRITE"] ) result |= SHF_WRITE;
    if ( sh_flags["SHF_ALLOC"] ) result |= SHF_ALLOC;
    if ( sh_flags["SHF_EXECINSTR"] ) result |= SHF_EXECINSTR;
    return result;
}
