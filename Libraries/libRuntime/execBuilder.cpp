#include "execBuilder.h"
ExecBuilder::ExecBuilder(FileLikeObject& f, ElfContent& c)
     : content(c), 
       reader(f), 
       writer(f),
       symTabHeader(*c.GetSection(".symtab")) { }

void ExecBuilder::MakeDataExecutable() {

    // Convienient aliases that will compile away
    ElfHeaderX86_64& header = content.header;
    auto& progHeaders = content.progHeaders;
    auto& psize = header.ProgramHeaderSize();

    // Helper objects
    BinaryWriter pwriter = writer + header.ProgramHeadersStart();

    for ( int i=0; i<header.ProgramHeaders(); i++ ) {
        RawProgramHeader& segment = (progHeaders[i])->RawHeader();

        // Look for the writeable load segment
        if (segment.IsLoadableSegment() && segment.IsWriteable() ) {
            // Update the parser copy
            segment.AddExecutable();
            // Update the file copy
            (pwriter+i*psize) << (Elf64_Phdr&) segment;
        }

    }
}

long ExecBuilder::InflateData(long newAllocation) {
    // Convienient aliases that will compile away
    ElfHeaderX86_64& header = content.header;
    auto& headersArray = content.progHeaders;

    // Utility objects
    BinaryWriter progWriter = writer + header.ProgramHeadersStart();

    unsigned long oldStopAddress=0xFFFFFFFFFFF;
    bool allocated = false;

    for ( int i=0; i<header.ProgramHeaders(); i++ ) {
        ProgramHeader& segment = *headersArray[i];

        if ( !allocated) {
            if (segment.IsLoadableSegment() && segment.IsWriteable() ) {
                oldStopAddress =   segment.Address() 
                                 + segment.SizeInMemory();
                segment.SizeInMemory() += newAllocation;
                allocated = true;
            }
        } else {
            // We to nudge the start addresses forward
            if ( segment.Address() >= oldStopAddress ) {
                segment.Address() += newAllocation;
            }
        }

        progWriter << (Elf64_Phdr&&) segment.RawHeader(); 
    }

    return allocated? oldStopAddress: 0;
}

// Update the value of an existing symbol in a file
bool ExecBuilder::UpdateSymbolValue(string name, long value) {

    bool ret = false;
    BinaryWriter symWrite = writer + symTabHeader.DataStart();

    auto loc = content.symbolMap.find(name);
    if ( loc != content.symbolMap.end() ) {
        int& idx = loc->second;

        // Update the cache
        Symbol& sym = *(content.symbols[idx]);
        sym.Value() = value;

        // Now update the file
        (symWrite + (idx * symTabHeader.ItemSize())) << sym.RawItem();

        ret = true;
    }
    return ret;
}
