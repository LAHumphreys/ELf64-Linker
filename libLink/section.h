#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
using namespace std;
#ifndef SectionX86_64
   #define SectionX86_64
#endif

class StringTable;

class Section {
    friend class X86Parser;
public:
    Section (string header, StringTable* );
    Elf64_Xword GetFlags();


private:
    char * data;
    Elf64_Shdr elfHeader;
    StringTable *stringTable;
    Flags sh_flags;
    /* data */
};

