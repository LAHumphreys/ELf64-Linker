#include "programHeader.h"
#include "section.h"
#include <sstream>

using namespace std;

ProgramHeader::ProgramHeader ( ElfReader& reader, 
                              long offset,
                              vector<Section *>& sections) 
                :flags("")
{
    reader.Read(offset,&data,Size());
    sectionNames = "";
    // While this isn't terribly efficient, there's only going to
    // be 20-30 sections a file...
    for ( auto s : sections) {
        if ( DataStart() <= s->DataStart() &&
             DataEnd()   >= s->DataEnd() )
        {
            sectionNames += s->Name() + " ";
        }
    }
    InitialiseFlags();
}

void ProgramHeader::InitialiseFlags() {
    flags.AddFlag('X', "Executable");
    flags.AddFlag('W', "Writeable");
    flags.AddFlag('R', "Readable");
    if ( data.p_flags & PF_X ) flags.SetFlag("X", 1);
    if ( data.p_flags & PF_W ) flags.SetFlag("W", 1);
    if ( data.p_flags & PF_R ) flags.SetFlag("R", 1);
}

// Format: align memsize addr flags sections..
string ProgramHeader::WriteLink() {
     ostringstream link;
     link << dec << Alignment() << " ";
     link << hex << SizeInMemory() << " ";
     link << hex << Address() << " ";
     link << flags.LinkMask() << " ";
     link << "\"" << sectionNames << "\"";
     return link.str();
}
