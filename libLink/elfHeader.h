// The linux kernel will define its own version of the assorted constants
// requried to build up the ELF format ( make sure you have the linux header
// files in your include path )
#include <elf.h>
#include <string>
#include <regex>

#ifndef ELFHEADERX86_64
   #define ELFHEADERX86_64 1
#endif

using namespace std;

enum Endian { LitleEndian, BigEndian};

class ElfHeaderX86_64 {
    friend class X86Parser;
public:
    ElfHeaderX86_64 (string linkHeader);
    void PopulateIdentity();
    void Write(ofstream &file);

private:
    Elf64_Ehdr data;
    Endian endian;
    //regex validate; // We can't use this until the people at gcc get their act together
    int nsections;
    int nprog;
    int nsyms;
    int nrelocs;
    char * readPtr;
};
