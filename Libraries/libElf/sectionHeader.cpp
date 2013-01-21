#include "sectionHeader.h"
#include <cstring>

SectionHeader::SectionHeader(const BinaryReader& loc) {
    loc.Read((Elf64_Shdr *)this,sizeof(Elf64_Shdr));
}

SectionHeader::SectionHeader(const Elf64_Shdr& rhs ): Elf64_Shdr(rhs) {}

SectionHeader::SectionHeader() {
     std::memset((Elf64_Shdr *)this,0,sizeof(Elf64_Shdr));
}
