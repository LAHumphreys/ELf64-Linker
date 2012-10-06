#ifndef SYMBOL_H
   #define SYMBOL_H
#include "flags.h"
#include "elf.h"
class BinaryPosition;

class Symbol {
public:
    Symbol ( const BinaryPosition& reader, 
             const BinaryPosition& stable );
    size_t Size() { return sizeof(Elf64_Sym); }
    Elf64_Addr& Value() { return symbol.st_value; }
    uint16_t& SegmentIdx() { return symbol.st_shndx; }
    void UpdateFlags();
    string LinkFormat();

private:
    void ConfigureFlags();
    Elf64_Sym symbol;
    string name;
    Flags type;
    Flags scope;
};
#endif
