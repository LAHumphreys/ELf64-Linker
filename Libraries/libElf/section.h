using namespace std;
#ifndef SectionX86_64
#define SectionX86_64
#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
#include "binaryData.h"
#include "sectionHeader.h"

class StringTable;
class BinaryReader;

class Section: public SectionHeader{
    friend class X86Parser;
public:
    Section( const BinaryReader& headerPos, 
             const BinaryReader &strings );
    Section (string header, StringTable* );
    ~Section ();
    Elf64_Xword GetFlags() ;

    // Methods
    string GetLinkFlags();
    string WriteLinkHeader();
    string WriteLinkData();

    void WriteRawData(BinaryWriter &writer) const;
    void WriteRawData(BinaryWriter &&writer) const {
        WriteRawData(writer);
    }

    bool IsLInkSection();
    string Name() { return name; }

    // The caller is repsonsible for destruction
    static Section* MakeNewStringTable( StringTable &tab, StringTable *sectionNames, string name);



protected:
    void ConfigureFlags();
    void SetFlags();
private:
    Section ();
    Data * data;
    StringTable *stringTable;
    string name;
    Flags sh_flags;
    /* data */
};

#endif
