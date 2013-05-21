#include "programHeader.h"
#include "section.h"
#include <sstream>
#include "binaryReader.h"
#include <algorithm>
#include <memory>

using namespace std;

RawProgramHeader::RawProgramHeader(const Elf64_Phdr& other) 
    : Elf64_Phdr(other) {}

ProgramHeader::ProgramHeader ( BinaryReader& reader, 
                               const SECTION_ARRAY& sections ) 
    :flags("")
{
    reader >> (Elf64_Phdr&)(*this);

    // While this isn't terribly efficient, there's only going to
    // be 20-30 sections a file...
    for ( auto s : sections) {
        if ( DataStart() <= s->DataStart() &&
             DataEnd()   >= s->DataEnd() )
        {
            sectionNames.insert(sectionNames.begin(), s->Name());
        }
    }
    InitialiseFlags();
}

Flags::Mask ProgramHeader::Flags_Executable  = Flags::EmptyMask;
Flags::Mask ProgramHeader::Flags_Writeable   = Flags::EmptyMask;
Flags::Mask ProgramHeader::Flags_Readable    = Flags::EmptyMask;

const Flags& ProgramHeader::TypeFlags() {
    static unique_ptr<Flags> flags = NULL;

    if ( !flags) {
        flags = unique_ptr<Flags>(new Flags(""));

        ProgramHeader::Flags_Executable = 
            flags->AddFlag('X', "Executable");
        ProgramHeader::Flags_Writeable = 
            flags->AddFlag('W', "Writeable");
        ProgramHeader::Flags_Readable = 
            flags->AddFlag('R', "Readable");
    }
    return *flags;
}

void ProgramHeader::InitialiseFlags() {
    if ( this->p_flags & PF_X ) 
        flags.SetFlags(Flags_Executable, 1);
    if ( this->p_flags & PF_W ) 
        flags.SetFlags(Flags_Writeable, 1);
    if ( this->p_flags & PF_R ) 
        flags.SetFlags(Flags_Readable, 1);
}

// Format: align memsize addr flags sections..
string ProgramHeader::WriteLink() {
     ostringstream link;
     link << dec << Alignment() << " ";
     link << hex << SizeInMemory() << " ";
     link << hex << Address() << " ";
     link << flags.LinkMask() << " ";
       
     link << "\"";
     for_each( sectionNames.begin(), 
               sectionNames.end(), 
               [&] (string &s) { link << name << " "; });
     link << "\"";
     return link.str();
}

RawProgramHeader ProgramHeader::RawHeader() {
    return *this;
}
