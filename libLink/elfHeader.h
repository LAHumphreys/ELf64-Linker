// The linux kernel will define its own version of the assorted constants
// requried to build up the ELF format ( make sure you have the linux header
// files in your include path )
#include <elf.h>
#include <string>
#include <vector>
#include "section.h"
#include "elfReader.h"

#ifndef ELFHEADERX86_64
   #define ELFHEADERX86_64 1
#endif

using namespace std;

enum Endian { LitleEndian, BigEndian};

class ElfHeaderX86_64 {
    friend class X86Parser;
public:
    ElfHeaderX86_64 (string linkHeader);
    ElfHeaderX86_64 (ElfReader &reader, int offset);
    void PopulateIdentity();
    void Write(ofstream &file);

    // Proivde a pointer that a new header may be read into
    size_t Size();

    // Data
    uint16_t& Sections() { return data.e_shnum; }
    Elf64_Off& SectionTableStart() { return data.e_shoff; }
    uint16_t& SectionHeaderSize() { return data.e_shentsize; }
    uint16_t& StringTableIndex() { return data.e_shstrndx; }



private:
    Elf64_Ehdr data;
    Endian endian;
    char * readPtr;
    std::vector<Section*> sections;
};
