#ifndef SectionX86_64
   #include "section.h"
#endif
#include <sstream>
#include "stringTable.h"
#include "binaryReader.h"

Section::Section( const BinaryPosition& headerPos, 
                  const BinaryPosition& strings ) 
    : sh_flags("")
{
    // over write the data in our data POD 
    headerPos.Read(&elfHeader,Size());
    // Get our name
    name = (strings + elfHeader.sh_name).ReadString();

    // Get our data
    data = new Data(headerPos.Reader().Begin() + DataStart() ,
                    DataSize());

}

Section::~Section () {
    if (data) delete data;
}

Section::Section(string header, StringTable * stable)
    : sh_flags(""), data(NULL) {
    sh_flags.AddFlag('W', "SHF_WRITE");
    sh_flags.AddFlag('A', "SHF_ALLOC");
    sh_flags.AddFlag('C', "SHF_EXECINSTR");
    this->stringTable = stable;

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

    elfHeader.sh_name = stringTable->AddString(name.c_str());
    elfHeader.sh_type = SHT_PROGBITS; // sym tables etc may override
    elfHeader.sh_flags = GetFlags();
    elfHeader.sh_addr = addr;
    elfHeader.sh_offset = 0; // we don't know this yet
    elfHeader.sh_size = size;
    elfHeader.sh_addralign = align;
    // only used for specialisations with fixed sized entries
    // (tables)
    elfHeader.sh_entsize = 0; 
    
    // These don't appear to be used
    elfHeader.sh_link = 0;
    elfHeader.sh_info = 0;
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
    return flags;
}
