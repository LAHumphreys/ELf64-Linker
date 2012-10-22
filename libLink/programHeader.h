#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
using namespace std;
#ifndef ProgramX86_64
#define ProgramX86_64

class Section;
class BinaryReader;
class ProgramHeader {
public:
    ProgramHeader ( const BinaryReader&, 
                    const vector< Section *>& sections );
    virtual ~ProgramHeader (){};

    // Methods
    string WriteLink();
    void InitialiseFlags();
    Elf64_Phdr RawHeader();
      
    // attributes
    size_t Size() { return sizeof(Elf64_Phdr);}
    Elf64_Off& DataStart() { return data.p_offset; }
    Elf64_Xword& Alignment() { return data.p_align; }
    Elf64_Xword& FileSize() { return data.p_filesz; }
    Elf64_Xword& SizeInMemory() { return data.p_memsz; }
    Elf64_Addr& Address() { return data.p_vaddr; }

    const std::vector<string>& SectionNames(){return sectionNames;}

    // Calculated values
    Elf64_Off DataEnd() { return DataStart() + FileSize(); }

private:
    Elf64_Phdr data;
    std::vector<string> sectionNames;
    string name;
    Flags flags;
};


#endif

