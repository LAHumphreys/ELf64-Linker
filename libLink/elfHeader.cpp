#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include "elfHeader.h"

using namespace std;
ElfHeaderX86_64::ElfHeaderX86_64 (const BinaryReader &reader)
{
    // over write the data in our data POD 
    reader.Read(&data,this->Size());
}
ElfHeaderX86_64::ElfHeaderX86_64 (string linkHeader)
     /*:validate("^ *[0-9]+ +[0-9]+ +[0-9]+$") */ {
    if ( false )  { // regex validation of linkHeader to prevent nastyness
        throw "Invalid LINK header: " + linkHeader;
    }
    long nsections;
    long nprog;
    long nsyms;
    long nrelocs;
    readPtr = (char * ) & data;
    stringstream s(linkHeader);
    s >> nsections;
    s >> nsyms;
    s >> nrelocs;


    endian = LitleEndian;
    PopulateIdentity();
    data.e_type = ET_EXEC;
    data.e_machine= EM_X86_64;
    data.e_version= EV_CURRENT;
    // not defined
    data.e_flags = 0;

    // header sizes
    data.e_ehsize = sizeof(Elf64_Ehdr);
    data.e_phentsize = sizeof(Elf64_Phdr);
    data.e_shentsize = sizeof(Elf64_Shdr);
    // --------------------------
    // We need knowledge of the rest of the file to handle these

    // offsets
    data.e_entry=0;
    data.e_phoff=0;
    data.e_shoff=0;

    // section counts
    data.e_phnum = 0; // # prog header: (used by the OS to load). Here the sections are consolidated by flag. 
    data.e_shnum = nsections; // # number of sections

    // ------------------------
}

void ElfHeaderX86_64::PopulateIdentity() {
    memcpy(data.e_ident,&ELFMAG,4);
    //ELF class
    data.e_ident[4] = ELFCLASS64;
    //Data format
    if ( endian == LitleEndian ) {
        data.e_ident[5] = ELFDATA2LSB;
    } else if (endian == BigEndian ) {
        data.e_ident[5] = ELFDATA2MSB;
    } else {
        throw "Endian is undefined";
    }
    // Elf version
    data.e_ident[6] = EV_CURRENT;

    memset(data.e_ident+7,0,EI_NIDENT - 7);
}

string ElfHeaderX86_64::LinkFlags() {
    string flags = "";
    if (data.e_ident[5] == ELFDATA2LSB) flags = "2L";
    if (data.e_ident[5] == ELFDATA2MSB) flags = "2B";
    return flags;
}

void ElfHeaderX86_64::Write(ofstream &file) {
    file.write(readPtr,sizeof(data));
}

size_t ElfHeaderX86_64::Size() {
    return sizeof(Elf64_Ehdr);
}
