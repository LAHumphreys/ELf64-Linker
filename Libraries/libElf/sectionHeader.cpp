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
    description << "sh_name:    : " << hex << sh_name << endl;
    description << "sh_type     : " << hex << sh_type << endl;
    description << "sh_flags    : " << hex << sh_flags << endl;	
    description << "sh_addr     : " << hex << sh_addr << endl;
    description << "sh_offset   : " << hex << sh_offset << endl;
    description << "sh_size     : " << hex << sh_size << endl;
    description << "sh_link     : " << hex << sh_link << endl;
    description << "sh_info     : " << hex << sh_info << endl;
    description << "sh_addralign: " << hex << sh_addralign << endl;
    description << "sh_entsize  : " << hex << sh_entsize << endl;	
    return description.str();
}
