#include "programHeader.h"
#include "section.h"
#include <sstream>
#include "binaryReader.h"
#include <algorithm>
#include <memory>
#include <iomanip>

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

string RawProgramHeader::Describe() const {
    stringstream description;
    description << "DataStart: ";
    description << hex << DataStart() << endl;

    description << "Alignment: ";
    description << hex << Alignment() << endl;

    description << "FileSize: ";
    description << hex << FileSize() << endl;

    description << "Size In Memory: ";
    description << hex << SizeInMemory() << endl;

    description << "Address: ";
    description << hex << Address() << endl;

    description << "Is Null: ";
    description << IsNull() << endl;

    description << "Is Loadable Segment: ";
    description << IsLoadableSegment() << endl;

    description << "Is Dynamic Linking: ";
    description << IsDynamicLinking() << endl;

    description << "Is Interpreter: ";
    description << IsInterpreter() << endl;

    description << "Is Note: ";
    description << IsNote() << endl;

    description << "Is Program Headers: ";
    description << IsProgramHeaders() << endl;

    description << "Is Thread Local Storage: ";
    description << IsThreadLocalStorage() << endl;

    description << "GNUIs Exception Frame: ";
    description << GNUIsExceptionFrame() << endl;

    description << "GNUIs Executable: ";
    description << GNUIsExecutable() << endl;

    description << "GNUIs Read Only After Reloc: ";
    description << GNUIsReadOnlyAfterReloc() << endl;

    description << "Is Executable: ";
    description << IsExecutable() << endl;

    description << "Is Readable: ";
    description << IsReadable() << endl;

    description << "Is Writeable: ";
    description << IsWriteable() << endl;

    return description.str();
}

int RawProgramHeader::FileRank() const {
    if ( IsProgramHeaders() )  {
        return 10;
    } else if ( IsInterpreter() ) {
        return 20;
    } else if ( IsLoadableSegment() ) {
        return 30;
    } else if ( IsDynamicLinking() ) {
        return 40;
    } else if ( IsNote() ) {
        return 50;
    } else if ( GNUIsExceptionFrame() ) {
        return 1000;
    } else if ( GNUIsExecutable() ) {
        return 1010;
    } else if ( GNUIsReadOnlyAfterReloc() ) {
        return 1020;
    } else {
        // By default, strange segments can be tagged on to the end
        return 99999999;
    }
    // Can't get here
}

void ProgramHeader::SetAdditionalMemory( const Elf64_Off& space ) {
        RawProgramHeader::SizeInMemory() = FileSize() + space;
}

const Elf64_Off ProgramHeader::GetAdditionalMemory() const 
{ 
    return SizeInMemory() - FileSize();
}

void ProgramHeader::SetFileSize( const Elf64_Off& fileSize ) {

	Elf64_Off fileSizeDelta = FileSize() - fileSize;
    RawProgramHeader::SizeInMemory() += fileSizeDelta;
    RawProgramHeader::FileSize() = fileSize;
}
