#ifndef SectionX86_64
   #include "section.h"
#endif
#include <sstream>
#include "stringTable.h"
#include "binaryReader.h"
#include "dataVector.h"
#include "binaryData.h"

Section::Section( ) 
    : sh_flags(""), stringTable(NULL), data(NULL)
{
    ConfigureFlags();
}

Section::Section( const BinaryReader& headerPos, 
                  const BinaryReader& strings ) 
    : SectionHeader(headerPos), sh_flags("")
{
    // Get our name
    name = (strings + (long)NameOffset()).ReadString();

    // Get our data
    data = new Data( headerPos.Begin().operator+(DataStart()) , 
                     DataSize());

    ConfigureFlags();
    SetFlags();
}


void Section::ConfigureFlags() {
    sh_flags.AddFlag('W', "SHF_WRITE");
    sh_flags.AddFlag('A', "SHF_ALLOC");
    sh_flags.AddFlag('C', "SHF_EXECINSTR");
}

void Section::SetFlags() {
    if ( Writeable() ) 
        sh_flags.SetFlag("SHF_WRITE",true);
    if ( Allocate() ) 
        sh_flags.SetFlag("SHF_ALLOC",true);
    if ( Executable() ) 
        sh_flags.SetFlag("SHF_EXECINSTR",true);
}

Section::~Section () {
    if (data) delete data;
}

Section::Section(string header, StringTable * stable)
    : sh_flags(""), data(NULL) {
    this->stringTable = stable;
    ConfigureFlags();

    // Build the flags
    stringstream s(header);
    string flags;
    Elf64_Addr addr;
    Elf64_Xword size;
    Elf64_Xword align;

    s >> name;
    s >> hex >> addr;
    s >> hex >> size;
    s >> flags;
    sh_flags.SetFlags(flags);
    s >> align;

    NameOffset() = stringTable->AddString(name.c_str());
    RawType() = SHT_PROGBITS; // sym tables etc may override
    RawFlags() = GetFlags();
    Address() = addr;
    DataStart() = 0; // we don't know this yet
    DataSize() = size;
    Alignment() = align;
    // only used for specialisations with fixed sized entries
    // (tables)
    ItemSize() = 0; 
    
    // These don't appear to be used
    RawLink() = 0;
    RawInfo() = 0;
}


Elf64_Xword Section::GetFlags() {
    // As it happens, the internal mask of our Flags object is
    // exactly this, but this safe guards against a change in elf.h
    Elf64_Xword result = 0;
    if ( sh_flags["SHF_WRITE"] ) result |= SHF_WRITE;
    if ( sh_flags["SHF_ALLOC"] ) result |= SHF_ALLOC;
    if ( sh_flags["SHF_EXECINSTR"] ) result |= SHF_EXECINSTR;
    return result;
}

// Expected format: "name value segmentIndex type scope"
//"name", "address", "size" , "flags", "alignment"
string Section::WriteLinkHeader() {
    ostringstream link;
    link << name << " ";
    link << hex << Address() << " ";
    link << hex << DataSize() << " ";
    link << GetLinkFlags() << " ";
    link << dec << Alignment() << " ";
    return link.str();
}

string Section::WriteLinkData() {
    ostringstream linkdata;
    linkdata << "# Data for section " << name << endl;
    linkdata << data->HexCode() ;
    return linkdata.str();
}

string Section::GetLinkFlags() {
    string flags = sh_flags.LinkMask();
    if ( HasFileData() ) {
        flags += "P";
    }
    flags += "R"; // everything is readable
    return flags;
}

bool Section::IsLInkSection() {
    return not (IsSymTable() || IsStringTable() || IsRelocTable() 
                || name == "");

}

Section * Section::MakeNewStringTable( StringTable &tab, 
                                       StringTable *sectionNames)
{
    Section * newSection = new Section();
    newSection->data = new Data(tab.Size());
    tab.WriteTable(newSection->data->Writer());
    newSection->stringTable = sectionNames;

    newSection->sh_flags.SetFlags("W");

    //build the elf header
    newSection->ItemSize() = 0; //no fixed sized entries
    newSection->RawFlags() = newSection->GetFlags();
    newSection->Alignment() = 0; //no addresses
    newSection->RawInfo() = 0; // man elf ( not 4 strings)
      
    // TODO: Handle for long files
    newSection->RawLink() = 0; 

    newSection->NameOffset() = 
       newSection->stringTable->AddString(newSection->name.c_str());
    newSection->RawType() = SHT_STRTAB;
    newSection->DataSize() = newSection->data->Size();

    return newSection;
}

void Section::WriteRawData(BinaryWriter &writer) {
    writer.Write(data->Reader(),data->Size());
}
