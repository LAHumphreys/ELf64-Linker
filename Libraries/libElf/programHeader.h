#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
using namespace std;
#ifndef ProgramX86_64
#define ProgramX86_64


class Section;
class BinaryReader;

class RawProgramHeader: public Elf64_Phdr {
public:
    RawProgramHeader(const Elf64_Phdr& other);
    // attributes
    size_t Size() { return sizeof(Elf64_Phdr);}
    Elf64_Off& DataStart() { return this->p_offset; }
    Elf64_Xword& Alignment() { return this->p_align; }
    Elf64_Xword& FileSize() { return this->p_filesz; }
    Elf64_Xword& SizeInMemory() { return this->p_memsz; }
    Elf64_Addr& Address() { return this->p_vaddr; }
protected:
    // A blank header is meaningless, but OK, if a child class is
    // controlling construction
    RawProgramHeader() = default;
};

class ProgramHeader: public RawProgramHeader {
public:
    typedef std::vector<Section*> SECTION_ARRAY;
    ProgramHeader ( BinaryReader&& r, const SECTION_ARRAY& s)
        : ProgramHeader(r,s){};
    ProgramHeader ( BinaryReader&, 
                    const SECTION_ARRAY&);
    virtual ~ProgramHeader (){};

    // Methods
    string WriteLink();
    void InitialiseFlags();
    Elf64_Phdr RawHeader();
      

    const std::vector<string>& SectionNames(){return sectionNames;}

    // Calculated values
    Elf64_Off DataEnd() { return DataStart() + FileSize(); }

private:
    std::vector<string> sectionNames;
    string name;
    Flags flags;
};


#endif

