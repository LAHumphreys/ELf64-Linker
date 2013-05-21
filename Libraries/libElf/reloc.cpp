#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
#include "binaryReader.h"
#include "reloc.h"
#include <memory>

using namespace std;

Relocation::Relocation ( const BinaryReader &reader, 
                         const string section)
    : type(TypeFlags())
{
    reader.Read(&reloc, Size());
    this->section = section;
}

Flags::Mask Flags_SHF_WRITE     = Flags::EmptyMask;
Flags::Mask Flags_SHF_ALLOC     = Flags::EmptyMask;
Flags::Mask Flags_SHF_EXECINSTR = Flags::EmptyMask;
Flags::Mask Flags_Absolute      = Flags::EmptyMask;
Flags::Mask Flags_Relative      = Flags::EmptyMask;
Flags::Mask Flags_Symbol        = Flags::EmptyMask;
Flags::Mask Flags_1Byte         = Flags::EmptyMask;
Flags::Mask Flags_2Byte         = Flags::EmptyMask;
Flags::Mask Flags_4Byte         = Flags::EmptyMask;
Flags::Mask Flags_8Byte         = Flags::EmptyMask;
Flags::Mask Flags_HasAddendum   = Flags::EmptyMask;
Flags::Mask Flags_ZeroExtended  = Flags::EmptyMask;
Flags::Mask Flags_SignExtended  = Flags::EmptyMask;

const Flags& Relocation::TypeFlags() {
    static unique_ptr<Flags> flags = NULL;

    if ( !flags) {
        flags = unique_ptr<Flags>(new Flags(""));

        Flags_SHF_WRITE    =
            flags->AddFlag('W', "SHF_WRITE");
        Flags_SHF_ALLOC    =
            flags->AddFlag('A', "SHF_ALLOC");
        Flags_SHF_EXECINSTR=
            flags->AddFlag('C', "SHF_EXECINSTR");
        Flags_Absolute     =
            flags->AddFlag('A',"Absolute");
        Flags_Relative     =
            flags->AddFlag('R',"Relative");
        Flags_Symbol       =
            flags->AddFlag('S',"Symbol");
        Flags_1Byte        =
            flags->AddFlag('1',"1Byte");
        Flags_2Byte        =
            flags->AddFlag('2',"2Byte");
        Flags_4Byte        =
            flags->AddFlag('4',"4Byte");
        Flags_8Byte        =
            flags->AddFlag('8',"8Byte");
        Flags_HasAddendum  =
            flags->AddFlag('+',"HasAddendum");
        Flags_ZeroExtended =
            flags->AddFlag('Z',"ZeroExtended");
        Flags_SignExtended =
            flags->AddFlag('I',"SignExtended");
    }
    return *flags;
}


void Relocation::ConvertFromElf () {
    // In this type all relocations are for symbols
    type.SetFlags(Flags_Symbol,true);

    // Calculate the other properties
    string error;
    switch(ElfRelocType()) {
    case R_X86_64_64: // * Direct 64 bit  */
         x86_type="R_X86_64_64 ";
         type.SetFlags(Flags_Absolute,true);
         type.SetFlags(Flags_8Byte,true);
         break;
    case R_X86_64_PC32:
         // this is an offset from the instruction pointer
         x86_type="R_X86_64_PC32"; // PC relative 32 bit signed
         type.SetFlags(Flags_Relative,true);
         type.SetFlags(Flags_4Byte,true);
         break;
    case R_X86_64_32:
	     // Loaded into a register and zero extended into a 
         // virtual address
         x86_type="R_X86_64_32"; //Direct 32 bit zero extended
         type.SetFlags(Flags_Absolute,true);
         type.SetFlags(Flags_4Byte,true);
         type.SetFlags(Flags_ZeroExtended,true);
         break;
    case R_X86_64_32S:
	     // Loaded into a register and sign extended into a 
         // virtual address
         x86_type="R_X86_64_32S";//Direct 32 bit sign extended
         type.SetFlags(Flags_Absolute,true);
         type.SetFlags(Flags_4Byte,true);
         type.SetFlags(Flags_SignExtended,true);
         break;
    case R_X86_64_16:
         x86_type="R_X86_64_16"; //Direct 16 bit zero extended
         type.SetFlags(Flags_Absolute,true);
         type.SetFlags(Flags_2Byte,true);
         type.SetFlags(Flags_ZeroExtended,true);
         break;
    case R_X86_64_PC16:
         x86_type="R_X86_64_PC16"; //16 bit sign extended pc relative
         type.SetFlags(Flags_Relative,true);
         type.SetFlags(Flags_SignExtended,true);
         type.SetFlags(Flags_2Byte,true);
         break;
    case R_X86_64_8:
         x86_type="R_X86_64_8"; //Direct 8 bit sign extended 
         type.SetFlags(Flags_Absolute,true);
         type.SetFlags(Flags_SignExtended,true);
         type.SetFlags(Flags_1Byte,true);
         break;
    case R_X86_64_PC8:
         x86_type="R_X86_64_PC8"; //8 bit sign extended pc relative
         type.SetFlags(Flags_Relative,true);
         type.SetFlags(Flags_SignExtended,true);
         type.SetFlags(Flags_1Byte,true);
         break;

    case R_X86_64_RELATIVE:
    case R_X86_64_GOT32:
    case R_X86_64_PLT32:
    case R_X86_64_COPY:
    case R_X86_64_GLOB_DAT:
    case R_X86_64_JUMP_SLOT:
    case R_X86_64_GOTPCREL:
        error = "Currently this relocation script does not ";
        error += "handle library specific types - ";
        error += "Global object tables etc ";
        throw error; 
        break;
    default:
        string error;
        error = "I'm sorry: I have no idea what this";
        error += "type of location is";
        throw error; 
    }

}
