#include "symbol.h"
#include <sstream>
#include "binaryReader.h"
#include <memory>
using namespace std;


Symbol::Symbol ( BinaryReader& reader,
                 BinaryReader& stable )
: type(TypeFlags()), scope(ScopeFlags()) { 
    reader >> (RawSymbol&) *this;

    // pull our name out of the string table
    name = (stable + st_name).ReadString();
    UpdateFlags();

}

const Flags& Symbol::TypeFlags() {
    static unique_ptr<Flags> flags = NULL;

    if ( !flags) {
        flags = unique_ptr<Flags>(new Flags(""));

        flags->AddFlag('U',"STT_NOTYPE");
        flags->AddFlag('O',"STT_OBJECT");
        flags->AddFlag('P',"STT_FUNC");
        flags->AddFlag('S',"STT_SECTION");
        flags->AddFlag('F',"STT_FILE");
    }
    return *flags;
}

const Flags& Symbol::ScopeFlags() {
    static unique_ptr<Flags> flags = NULL;

    if ( !flags) {
        flags = unique_ptr<Flags>(new Flags(""));

        flags->AddFlag('L',"STB_LOCAL");
        flags->AddFlag('G',"STB_GLOBAL");
        flags->AddFlag('W',"STB_WEAK");
    }

    return *flags;
}

void Symbol::UpdateFlags() {
    unsigned char infoMask = st_info;
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
    line << dec <<  SectionIndex() << " ";
    line << type.LinkMask() << " " << scope.LinkMask();
    return line.str();
}

bool Symbol::IsLinkSymbol () {
    return name != "";
}
