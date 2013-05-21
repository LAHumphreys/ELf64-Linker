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

Flags::Mask Symbol::Flags_STT_NOTYPE = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STT_OBJECT = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STT_FUNC = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STT_SECTION = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STT_FILE = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STB_LOCAL = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STB_GLOBAL = Flags::EmptyMask;
Flags::Mask Symbol::Flags_STB_WEAK = Flags::EmptyMask;

const Flags& Symbol::TypeFlags() {
    static unique_ptr<Flags> flags = NULL;

    if ( !flags) {
        flags = unique_ptr<Flags>(new Flags(""));

        Flags_STT_NOTYPE = 
            flags->AddFlag('U',"STT_NOTYPE");
        Flags_STT_OBJECT = 
            flags->AddFlag('O',"STT_OBJECT");
        Flags_STT_FUNC = 
            flags->AddFlag('P',"STT_FUNC");
        Flags_STT_SECTION = 
            flags->AddFlag('S',"STT_SECTION");
        Flags_STT_FILE = 
            flags->AddFlag('F',"STT_FILE");
    }
    return *flags;
}

const Flags& Symbol::ScopeFlags() {
    static unique_ptr<Flags> flags = NULL;

    if ( !flags) {
        flags = unique_ptr<Flags>(new Flags(""));

        Flags_STB_LOCAL = 
            flags->AddFlag('L',"STB_LOCAL");
        Flags_STB_GLOBAL = 
            flags->AddFlag('G',"STB_GLOBAL");
        Flags_STB_WEAK = 
            flags->AddFlag('W',"STB_WEAK");
    }

    return *flags;
}

void Symbol::UpdateFlags() {
    unsigned char infoMask = st_info;
    unsigned char bindMask = ELF64_ST_BIND(infoMask);
    unsigned char typeMask = ELF64_ST_TYPE(infoMask);
    type.SetFlags(Flags_STT_NOTYPE,  typeMask == STT_NOTYPE );
    type.SetFlags(Flags_STT_OBJECT,  typeMask == STT_OBJECT );
    type.SetFlags(Flags_STT_FUNC,    typeMask == STT_FUNC );
    type.SetFlags(Flags_STT_SECTION, typeMask == STT_SECTION );
    type.SetFlags(Flags_STT_FILE,    typeMask == STT_FILE ); 
    scope.SetFlags(Flags_STB_LOCAL,   bindMask == STB_LOCAL );
    scope.SetFlags(Flags_STB_GLOBAL,  bindMask == STB_GLOBAL );
    scope.SetFlags(Flags_STB_WEAK,    bindMask == STB_WEAK );
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
