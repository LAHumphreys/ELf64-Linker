#ifndef SYMBOL_H
   #define SYMBOL_H
#include "flags.h"
#include "elf.h"
class BinaryReader;

class RawSymbol: public Elf64_Sym { 
public:
    RawSymbol(Elf64_Sym& other):Elf64_Sym(other){}
    RawSymbol() = default;

    Elf64_Section& SectionIndex() { return st_shndx;}
    Elf64_Addr& Value() { return st_value;}
    Elf64_Xword& Size() { return st_size;}
};

class Symbol: protected RawSymbol {
public:
    Symbol ( BinaryReader& reader, 
             BinaryReader& stable );
    Symbol ( BinaryReader&& r, 
             BinaryReader&& s): Symbol(r,s){}
    bool IsLinkSymbol();
    void UpdateFlags();
    string LinkFormat();
    string Name() { return name;}

    using RawSymbol::Value;
    using RawSymbol::SectionIndex;
    using RawSymbol::Size;

    const RawSymbol& RawItem() { return *this;}

protected:
    // If we don't do this, we have to re-hash and bit manipulate the same
    // 8 strings for every single symbol object we instantiate
    static const Flags& TypeFlags();
    static const Flags& ScopeFlags();

    static Flags::Mask Flags_STT_NOTYPE;
    static Flags::Mask Flags_STT_OBJECT;
    static Flags::Mask Flags_STT_FUNC;
    static Flags::Mask Flags_STT_SECTION;
    static Flags::Mask Flags_STT_FILE;
    static Flags::Mask Flags_STB_LOCAL;
    static Flags::Mask Flags_STB_GLOBAL;
    static Flags::Mask Flags_STB_WEAK;

private:
    string name;
    Flags type;
    Flags scope;
};
#endif
