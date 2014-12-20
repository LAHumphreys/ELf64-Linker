#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"
#include <elf.h>
#include "dataLump.h"
#include "defer.h"
#include "stdReader.h"
#include "binaryDescribe.h"

int magic( testLogger& log);
int headerClass( testLogger& log);
int tEI_DATA (testLogger& log);
int tEI_VERSION (testLogger& log);
int PAD (testLogger& log);
int tEI_OSABI (testLogger& log);
int te_type(testLogger& log);
int machine(testLogger& log);
int HeaderSizes(testLogger& log );
int HeaderStarts(testLogger& log);
int Diff(testLogger& log);

string infile= "";


ElfParser *p;
unsigned char * buf = new unsigned char[50000000];
DEFER ( delete [] buf;)
DataIO outfile(buf,5000000);
Elf64_Ehdr hdr;
Elf64_Ehdr ohdr;
ElfHeaderX86_64 *header;
ElfHeaderX86_64* newHeader;

using namespace std;

/*
 * Validate a basic object file
 */
int main(int argc, const char *argv[])
{ 
    // Use ourself as the sample binary
    infile = argv[0];
    ElfFileReader f(argv[0]);
    
    p = new ElfParser(f);
    
    //parse the input file
    ElfContent content = p->Content();
    header = &(content.header);

    ElfFile file( content);
    
    // Write out the data
    file.WriteToFile(outfile);

    newHeader = new ElfHeaderX86_64(outfile);
    BinaryReader(outfile).Read(&hdr,sizeof(Elf64_Ehdr));
    BinaryReader(f).Read(&ohdr,sizeof(Elf64_Ehdr));

    Test("ELF MAGIC",  magic).RunTest();
    Test("ELF CLASS",  headerClass).RunTest();
    Test("ELF VERSION",  tEI_VERSION).RunTest();
    Test("ELF DATA TYPE",  tEI_DATA).RunTest();
    Test("ELF OS ABI",  tEI_OSABI).RunTest();
    Test("ELF PAD",  PAD).RunTest();
    Test("ELF TYPE",  te_type).RunTest();
    Test("ELF MACHINE",  machine).RunTest();
    Test("ELF HEADERS (SIZES)",  HeaderSizes).RunTest();
    Test("ELF HEADERS (STARTS)",  HeaderStarts).RunTest();
    Test("Diffing the headers...", Diff).RunTest();

    delete newHeader;
    delete p;

    return 0;
}

/*
 *
 */
int magic( testLogger& log) {
    log << "0: >" << ELFMAG0 << "< , " << hdr.e_ident[0] << endl;
    log << "1: >" << ELFMAG1 << "< , " << hdr.e_ident[1] << endl;
    log << "2: >" << ELFMAG2 << "< , " << hdr.e_ident[2] << endl;
    log << "3: >" << ELFMAG3 << "< , " << hdr.e_ident[3] << endl;
    if (    ELFMAG0 != hdr.e_ident[0] 
         || ELFMAG1 != hdr.e_ident[1] 
         || ELFMAG2 != hdr.e_ident[2]  
         || ELFMAG3 != hdr.e_ident[3]  ) {
            log << "MAGIC FAILED!" << endl;
            return 1;
    }
    return 0;
}

int headerClass (testLogger& log) {
    if ( hdr.e_ident[EI_CLASS] != ELFCLASS64 ) {
        log << "CLASS MISSMATCH" << endl;
        log << hdr.e_ident[EI_CLASS] << " , " << ELFCLASS64 << endl;
        return 1;
    } else {
        return 0;
    }
 }

int tEI_DATA (testLogger& log) {
    if ( hdr.e_ident[EI_DATA] != ELFDATA2LSB ) {
        log << "DATA MISSMATCH" << endl;
        log << hdr.e_ident[EI_DATA] << " , " << ELFDATA2LSB << endl;
        return 1;
    } else {
        return 0;
    }
 }

int tEI_VERSION (testLogger& log) {
    if ( hdr.e_ident[EI_VERSION] != EV_CURRENT ) {
        log << "VERSION MISSMATCH" << endl;
        log << hdr.e_ident[EI_VERSION] << " , " << EV_CURRENT << endl;
        return 1;
    } else {
        return 0;
    }
 }

int tEI_OSABI (testLogger& log) {
    if ( hdr.e_ident[EI_OSABI] != ELFOSABI_LINUX ) {
        log << "APPLICATION BINARY INTERFACE  MISSMATCH" << endl;
        log << hdr.e_ident[EI_OSABI] << " , " << ELFOSABI_LINUX << endl;
        return 1;
    } else {
        return 0;
    }
}

int PAD (testLogger& log) {
    for ( int i=EI_OSABI +1; i< EI_NIDENT; i++ ) {
        if ( hdr.e_ident[i] != 0 ) {
            log << "NON-ZERO PADDING in ELF_HEADER" << endl;
            log << hdr.e_ident[i] << " , " << 0 << endl;
            return 1;
        }
    }
    return 0;
}

int te_type(testLogger& log) {
    if ( hdr.e_type != ET_EXEC ) {
        log << "Invalid type for object file" << endl;
        log << hdr.e_type << " , " << ET_REL  << endl;
        return 1;
    } else {
        return 0;
    }
}

int machine(testLogger& log) {
    if ( hdr.e_machine != EM_X86_64 ) {
        log << "Unexpected machine type" << endl;
        log << hdr.e_machine << " , " << EM_X86_64  << endl;
        return 1;
    } else {
        return 0;
    }
}

int HeaderSizes(testLogger& log ) {
    if ( hdr.e_shentsize != sizeof(Elf64_Shdr) ) {
        log << "Invalid section header size" << endl;
        log << hdr.e_shentsize << " , ";
        log  << sizeof(Elf64_Shdr)  << endl;
        return 1;
    } else if ( hdr.e_phentsize != sizeof(Elf64_Phdr) ){
        log << "Invalid program header size" << endl;
        log << hdr.e_phentsize << " , ";
        log  << sizeof(Elf64_Phdr)  << endl;
        return 1;
    } else if ( hdr.e_shnum != ohdr.e_shnum ){
        log << "Unexpected change in # of sections" << endl;
        log << ohdr.e_shnum << " , ";
        log  << hdr.e_shnum << endl;
        return 1;
    } else if ( hdr.e_phnum != ohdr.e_phnum ){
        log << "Unexpected change in # of program section" << endl;
        log << ohdr.e_phnum << " , ";
        log  << hdr.e_phnum << endl;
        return 1;
    }
    return 0;
}

int HeaderStarts(testLogger& log ) {
    // Parsed File
    if ( header->SectionTableStart() != ohdr.e_shoff ) {
        log << "Missmatch in parsed section header start " << endl;
        log << newHeader->SectionTableStart() << ", ";
        log << ohdr.e_shoff << endl ;
        return 2;
    } else if ( header->ProgramHeadersStart() != ohdr.e_phoff ){
        log << "Missmatch in parsed program  headers start" << endl;
        log << ohdr.e_phoff << " -> "; 
        log << hdr.e_phoff;
        return 2;
    }

    // Written File
    if ( newHeader->ProgramHeadersStart() != ohdr.e_phoff ){
        log << "Missmatch in program headers start" << endl;
        log << hdr.e_phoff << " , ";
        log << ohdr.e_phoff << endl;;
        return 1;
    }
    return 0;
}

int Diff(testLogger& log) {
    /*
     * Read in the original header
     */
    Elf64_Ehdr originalHeader;
    Elf64_Ehdr newHeader;
    IFStreamReader originalFile(infile.c_str());
    BinaryReader(originalFile) >> originalHeader;
    BinaryReader(outfile) >> newHeader;

    if ( newHeader.e_ident[EI_MAG0] != originalHeader.e_ident[EI_MAG0] ) {
        log << "Differing MAG0" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_MAG1] != originalHeader.e_ident[EI_MAG1] ) {
        log << "Differing MAG1" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_MAG2] != originalHeader.e_ident[EI_MAG2] ) {
        log << "Differing MAG2" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_MAG3] != originalHeader.e_ident[EI_MAG3] ) {
        log << "Differing MAG3" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_CLASS] != originalHeader.e_ident[EI_CLASS] ) {
        log << "Differing CLASS" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_DATA] != originalHeader.e_ident[EI_DATA] ) {
        log << "Differing DATA" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_VERSION] != originalHeader.e_ident[EI_VERSION] ) {
        log << "Differing VERSION" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_OSABI] != originalHeader.e_ident[EI_OSABI] ) {
        log << "Differing OSABI" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_ABIVERSION] != originalHeader.e_ident[EI_ABIVERSION] ) {
        log << "Differing ABIVERSION" << endl;
        return 1;
    }

    if ( newHeader.e_ident[EI_PAD] != originalHeader.e_ident[EI_PAD] ) {
        log << "Differing PAD" << endl;
        return 1;
    }

    if ( newHeader.e_type != originalHeader.e_type) {
    	log << " Differing types! " << endl;
    	return 1;
    }

    if ( newHeader.e_entry != originalHeader.e_entry) {
    	log << " Differing entry points! " << endl;
    	return 1;
    }

    if ( newHeader.e_shstrndx != originalHeader.e_shstrndx)
    {
    	log << "Differing section header string indexes!"
    		<< originalHeader.e_shstrndx << " -> "
			<< newHeader.e_shstrndx << endl;
    	return 1;
    }
    return 0;
}
