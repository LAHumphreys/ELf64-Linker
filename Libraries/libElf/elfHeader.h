// The linux kernel will define its own version of the assorted constants
// requried to build up the ELF format ( make sure you have the linux header
// files in your include path )
#include <elf.h>
#include <string>
#include <vector>
#include "section.h"
#include "binaryReader.h"

#ifndef ELFHEADERX86_64
#define ELFHEADERX86_64 1

using namespace std;

enum Endian { LitleEndian, BigEndian};

class ElfHeaderX86_64 {
    friend class X86Parser;
public:
    ElfHeaderX86_64 (string linkHeader);
    ElfHeaderX86_64 (const BinaryReader& reader);
    ElfHeaderX86_64 (BinaryReader&& r): ElfHeaderX86_64(r) {}
    void PopulateIdentity();
    void PopulateLinuxConsts();
    void Write(ofstream &file);

    size_t Size();

    // Data
    uint16_t& ProgramHeaders() { return data.e_phnum; }
    Elf64_Off& ProgramHeadersStart() { return data.e_phoff; }
    uint16_t&  ProgramHeaderSize() { return data.e_phentsize; }
    uint16_t& Sections() { return data.e_shnum; }
    Elf64_Off& SectionTableStart() { return data.e_shoff; }
    uint16_t& SectionHeaderSize() { return data.e_shentsize; }
    uint16_t& StringTableIndex() { return data.e_shstrndx; }

    void GetHeader(Elf64_Ehdr& out);
    static ElfHeaderX86_64 NewObjectFile();

    // Calculated Flags
    string LinkFlags();



private:
    ElfHeaderX86_64 () {}
    Elf64_Ehdr data;
    Endian endian;
    char * readPtr;
    std::vector<Section*> sections;
};
#endif
