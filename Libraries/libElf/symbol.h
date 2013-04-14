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

private:
    void ConfigureFlags();
    string name;
    Flags type;
    Flags scope;
};
#endif
