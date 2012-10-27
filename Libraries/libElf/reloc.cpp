#include <string>
#include <vector>
#include "flags.h"
#include "elf.h"
#include "binaryReader.h"
#include "reloc.h"

using namespace std;

Relocation::Relocation ( const BinaryReader &reader, 
                         const string section)
    : type("")
{
    ConfigureFlags();
    reader.Read(&reloc, Size());
    this->section = section;
}

void Relocation::ConfigureFlags() {
    type.AddFlag('W', "SHF_WRITE");
    type.AddFlag('A', "SHF_ALLOC");
    type.AddFlag('C', "SHF_EXECINSTR");
    type.AddFlag('A',"Absolute");
    type.AddFlag('R',"Relative");
    type.AddFlag('S',"Symbol");
    type.AddFlag('1',"1Byte");
    type.AddFlag('2',"2Byte");
    type.AddFlag('4',"4Byte");
    type.AddFlag('8',"8Byte");
    type.AddFlag('+',"HasAddendum");
    type.AddFlag('Z',"ZeroExtended");
    type.AddFlag('I',"SignExtended");
}

void Relocation::ConvertFromElf () {
    // In this type all relocations are for symbols
    type.SetFlag("Symbol",true);

    // Calculate the other properties
    string error;
    switch(ElfRelocType()) {
    case R_X86_64_64: // * Direct 64 bit  */
         x86_type="R_X86_64_64 ";
         type.SetFlag("A",true);
         type.SetFlag("8",true);
         break;
    case R_X86_64_PC32:
         // this is an offset from the instruction pointer
         x86_type="R_X86_64_PC32"; // PC relative 32 bit signed
         type.SetFlag("R",true);
         type.SetFlag("4",true);
         break;
    case R_X86_64_32:
	     // Loaded into a register and zero extended into a 
         // virtual address
         x86_type="R_X86_64_32"; //Direct 32 bit zero extended
         type.SetFlag("A",true);
         type.SetFlag("4",true);
         type.SetFlag("Z",true);
         break;
    case R_X86_64_32S:
	     // Loaded into a register and sign extended into a 
         // virtual address
         x86_type="R_X86_64_32S";//Direct 32 bit sign extended
         type.SetFlag("A",true);
         type.SetFlag("4",true);
         type.SetFlag("I",true);
         break;
    case R_X86_64_16:
         x86_type="R_X86_64_16"; //Direct 16 bit zero extended
         type.SetFlag("A",true);
         type.SetFlag("2",true);
         type.SetFlag("Z",true);
         break;
    case R_X86_64_PC16:
         x86_type="R_X86_64_PC16"; //16 bit sign extended pc relative
         type.SetFlag("R",true);
         type.SetFlag("I",true);
         type.SetFlag("2",true);
         break;
    case R_X86_64_8:
         x86_type="R_X86_64_8"; //Direct 8 bit sign extended 
         type.SetFlag("A",true);
         type.SetFlag("I",true);
         type.SetFlag("1",true);
         break;
    case R_X86_64_PC8:
         x86_type="R_X86_64_PC8"; //8 bit sign extended pc relative
         type.SetFlag("R",true);
         type.SetFlag("I",true);
         type.SetFlag("1",true);
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
