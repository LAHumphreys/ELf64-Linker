#include "symbol.h"
#include <sstream>
#include "elfReader.h"
using namespace std;

Symbol::Symbol (ElfReader &reader, int offset, int stable)
: type(""), scope(""){ 
    ConfigureFlags();
    reader.Read(offset,&symbol,this->Size());
    // Get our name
    reader.ReadString( stable + symbol.st_name, name);
    UpdateFlags();

}

void Symbol::ConfigureFlags() {
    type.AddName('U',"STT_NOTYPE");
    type.AddName('O',"STT_OBJECT");
    type.AddName('P',"STT_FUNC");
    type.AddName('S',"STT_SECTION");
    type.AddName('F',"STT_FILE");

    scope.AddName('L',"STB_LOCAL");
    scope.AddName('G',"STB_GLOBAL");
    scope.AddName('W',"STB_WEAK");
}

void Symbol::UpdateFlags() {
    unsigned char infoMask = symbol.st_info;
    unsigned char bindMask = ELF64_ST_BIND(infoMask);
    unsigned char typeMask = ELF64_ST_TYPE(infoMask);
    type.SetFlag("STT_NOTYPE",  typeMask == STT_NOTYPE );
    type.SetFlag("STT_OBJECT",  typeMask == STT_OBJECT );
    type.SetFlag("STT_FUNC",    typeMask == STT_FUNC );
    type.SetFlag("STT_SECTION", typeMask == STT_SECTION );
    type.SetFlag("STT_FILE",    typeMask == STT_FILE ); 
    type.SetFlag("STB_LOCAL",   bindMask == STB_LOCAL );
    type.SetFlag("STB_GLOBAL",  bindMask == STB_GLOBAL );
    type.SetFlag("STB_WEAK",    bindMask == STB_WEAK );
}

string Symbol::LinkFormat() {
    ostringstream line;
    line << name << " ";
    line << hex <<  Value() << " ";
    line << dec <<  SegmentIdx() << " ";
    line << type.LinkMask() << " " << scope.LinkMask();
    return line.str();
}
