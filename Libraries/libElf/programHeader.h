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
    Elf64_Off& DataStart() { return p_offset; }
    Elf64_Xword& Alignment() { return p_align; }
    Elf64_Xword& FileSize() { return p_filesz; }
    Elf64_Xword& SizeInMemory() { return p_memsz; }
    Elf64_Addr& Address() { return p_vaddr; }

    // Check Flags
    bool IsExecutable() { return p_flags & PF_X; }
    bool IsProgramSegment() { return p_type == PT_LOAD;}
    bool IsReadable() { return p_flags & PF_R; }
    bool IsWriteable() { return p_flags & PF_W; }

    // Update Flags
    void AddExecutable() { p_type |= PF_X;}
    void AddReadable() { p_type |= PF_R;}
    void AddWriteable() { p_type |= PF_W;}
    void RemoveExecutable() { p_type &= ~PF_X; }
    void RemoveReadable() { p_type &= ~PF_R; }
    void RemoveWriteable() { p_type &= ~PF_W; }

protected:
    // Allow derived classes to contruct us when
    // they're ready - its only data
    RawProgramHeader() = default;
};

class ProgramHeader: private RawProgramHeader {
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
    RawProgramHeader RawHeader();
      

    const std::vector<string>& SectionNames(){return sectionNames;}

    // Calculated values
    Elf64_Off DataEnd() { return DataStart() + FileSize(); }

private:
    std::vector<string> sectionNames;
    string name;
    Flags flags;
};


#endif

