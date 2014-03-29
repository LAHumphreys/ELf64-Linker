#include "tester.h"
#include "elfParser.h"
#include "buildElf.h"
#include <elf.h>
#include "dataLump.h"
#include "defer.h"

int BinaryComp(testLogger& log );

int main(int argc, const char *argv[])
{
    
    // Check symbol table data matches
    Test("Checking Symnbols...",BinaryComp).RunTest();
    return 0;
}

int BinaryComp(testLogger& log ) {

    ElfFileReader f("../elf2elf/isYes/isYes.o");

    ElfParser p(f);

    Section& symTab = *p.Content().GetSection(".symtab");

    RawSymbol* oldSymbols = new RawSymbol[symTab.NumItems()];
    DEFER(delete [] oldSymbols;)

    BinaryReader oldReader(f, symTab.DataStart());
    for (size_t i=0; i< symTab.NumItems(); i++ ) {
        oldReader >> oldSymbols[i];
    }
    
    // OK now we re-build a new file
    ElfFile file( p.Content());
    DataLump<5000> outfile;

    file.WriteToFile(outfile);

    Section& nsymTab = *p.Content().GetSection(".symtab");

    RawSymbol* newSymbols = new RawSymbol[nsymTab.NumItems()];
    DEFER(delete [] newSymbols;)

    BinaryReader newReader(outfile, nsymTab.DataStart());
    for (size_t i=0; i< symTab.NumItems(); i++ ) {
        newReader >> newSymbols[i];
        string name = p.Content().symbols[i]->Name();
        log << "Checking symbol " << i << ": " << name << endl;
        log << "**************** OLD *****************" << endl;
        log << oldSymbols[i].Describe();
        log << "**************** NEW *****************" << endl;
        log << newSymbols[i].Describe();
        log << "**************************************" << endl;
        if (   newSymbols[i].SectionIndex() 
            != oldSymbols[i].SectionIndex() )
        {
            log << "Index missmatch in symbol " << i << endl;
            return 1;
        }

        if (   newSymbols[i].Value() 
            != oldSymbols[i].Value() )
        {
            log << "Value missmatch in symbol " << i << endl;
            return 2;
        }

        if (   newSymbols[i].Size() 
            != oldSymbols[i].Size() )
        {
            log << "Size missmatch in symbol " << i << endl;
            return 3;
        }
    }

    // OK now make sure they're what they should be
    return memcmp(oldSymbols, newSymbols,symTab.DataSize());
}
