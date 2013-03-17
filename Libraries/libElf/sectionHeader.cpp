#include "sectionHeader.h"
#include <cstring>
#include <sstream>

SectionHeader::SectionHeader(const BinaryReader& loc) {
    loc.Read((Elf64_Shdr *)this,sizeof(Elf64_Shdr));
}

SectionHeader::SectionHeader(const Elf64_Shdr& rhs ): Elf64_Shdr(rhs) {}

SectionHeader::SectionHeader() {
     std::memset((Elf64_Shdr *)this,0,sizeof(Elf64_Shdr));
}

string SectionHeader::Descripe () const {
    stringstream description;
    description << "sh_name:    : " << sh_name << endl;
    description << "sh_type     : " << sh_type << endl;
    description << "sh_flags    : " << sh_flags << endl;	
    description << "sh_addr     : " << sh_addr << endl;
    description << "sh_offset   : " << sh_offset << endl;
    description << "sh_size     : " << sh_size << endl;
    description << "sh_link     : " << sh_link << endl;
    description << "sh_info     : " << sh_info << endl;
    description << "sh_addralign: " << sh_addralign << endl;
    description << "sh_entsize  : " << sh_entsize << endl;	
    return description.str();
}
