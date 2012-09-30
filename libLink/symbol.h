#ifndef SYMBOL_H
   #define SYMBOL_H
#include "flags.h"
#include "elf.h"
class ElfReader;

class Symbol {
public:
    Symbol (ElfReader &reader, int offset, int stable);
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
