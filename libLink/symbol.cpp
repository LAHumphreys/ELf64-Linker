#include "symbol.h"
#include <sstream>
#include "binaryReader.h"
using namespace std;


Symbol::Symbol ( const BinaryPosition& reader,
                 const BinaryPosition& stable )
: type(""), scope(""){ 
    ConfigureFlags();
    reader.Read(&symbol,Size());
    // pull our name out of the string table
    name = (stable + symbol.st_name).ReadString();
    UpdateFlags();

}

void Symbol::ConfigureFlags() {
    type.AddFlag('U',"STT_NOTYPE");
    type.AddFlag('O',"STT_OBJECT");
    type.AddFlag('P',"STT_FUNC");
    type.AddFlag('S',"STT_SECTION");
    type.AddFlag('F',"STT_FILE");

    scope.AddFlag('L',"STB_LOCAL");
    scope.AddFlag('G',"STB_GLOBAL");
    scope.AddFlag('W',"STB_WEAK");
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
    scope.SetFlag("STB_LOCAL",   bindMask == STB_LOCAL );
    scope.SetFlag("STB_GLOBAL",  bindMask == STB_GLOBAL );
    scope.SetFlag("STB_WEAK",    bindMask == STB_WEAK );
}

string Symbol::LinkFormat() {
    ostringstream line;
    if ( name == "" ) {
        line <<  "__blank__ ";
    } else { 
        line << name << " ";
    }
    line << hex <<  Value() << " ";
    line << dec <<  SegmentIdx() << " ";
    line << type.LinkMask() << " " << scope.LinkMask();
    return line.str();
}

bool Symbol::IsLinkSymbol () {
    return name != "";
}
