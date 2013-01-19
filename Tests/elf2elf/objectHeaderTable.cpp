#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"
#include <elf.h>

int magic( stringstream& log);
int headerClass( stringstream& log);
int tEI_DATA (stringstream& log);
int tEI_VERSION (stringstream& log);
int PAD (stringstream& log);
int tEI_OSABI (stringstream& log);
int te_type(stringstream& log);
int machine(stringstream& log);
int HeaderSizes(stringstream& log );

ElfParser *p;
DataVector outfile(5000);
ElfHeaderX86_64 *header;
Elf64_Ehdr *hdr;

using namespace std;

/*
 * Validate a basic object file
 */
int main(int argc, const char *argv[])
{
    ElfFileReader f("../elf2elf/isYes/isYes.o");
    
    p = new ElfParser(f);
    
    //parse the input file
    ElfContent content = p->Content();
    ElfFile file( content);
    
    // Write out the data
    file.WriteToFile(outfile.Writer());

	header = new ElfHeaderX86_64(outfile.Reader());
	hdr = (Elf64_Ehdr *) outfile.Reader().Dup(sizeof(Elf64_Ehdr));

    Test("ELF MAGIC",  (loggedTest)magic).RunTest();
    Test("ELF CLASS",  (loggedTest)headerClass).RunTest();
    Test("ELF VERSION",  (loggedTest)tEI_VERSION).RunTest();
    Test("ELF DATA TYPE",  (loggedTest)tEI_DATA).RunTest();
    Test("ELF OS ABI",  (loggedTest)tEI_OSABI).RunTest();
    Test("ELF PAD",  (loggedTest)PAD).RunTest();
    Test("ELF TYPE",  (loggedTest)te_type).RunTest();
    Test("ELF MACHINE",  (loggedTest)machine).RunTest();
    Test("ELF HEADERS",  (loggedTest)HeaderSizes).RunTest();
	delete header;
	delete p;
    return 0;
}

/*
 *
 */
int magic( stringstream& log) {
	log << "0: >" << ELFMAG0 << "< , " << hdr->e_ident[0] << endl;
	log << "1: >" << ELFMAG1 << "< , " << hdr->e_ident[1] << endl;
	log << "2: >" << ELFMAG2 << "< , " << hdr->e_ident[2] << endl;
	log << "3: >" << ELFMAG3 << "< , " << hdr->e_ident[3] << endl;
	if (    ELFMAG0 != hdr->e_ident[0] 
		 || ELFMAG1 != hdr->e_ident[1] 
		 || ELFMAG2 != hdr->e_ident[2]  
		 || ELFMAG3 != hdr->e_ident[3]  ) {
			log << "MAGIC FAILED!" << endl;
			return 1;
	}
	return 0;
}

int headerClass (stringstream& log) {
	if ( hdr->e_ident[EI_CLASS] != ELFCLASS64 ) {
		log << "CLASS MISSMATCH" << endl;
		log << hdr->e_ident[EI_CLASS] << " , " << ELFCLASS64 << endl;
		return 1;
	} else {
	    return 0;
	}
 }

int tEI_DATA (stringstream& log) {
	if ( hdr->e_ident[EI_DATA] != ELFDATA2LSB ) {
		log << "DATA MISSMATCH" << endl;
		log << hdr->e_ident[EI_DATA] << " , " << ELFDATA2LSB << endl;
		return 1;
	} else {
	    return 0;
	}
 }

int tEI_VERSION (stringstream& log) {
	if ( hdr->e_ident[EI_VERSION] != EV_CURRENT ) {
		log << "VERSION MISSMATCH" << endl;
		log << hdr->e_ident[EI_VERSION] << " , " << EV_CURRENT << endl;
		return 1;
	} else {
	    return 0;
	}
 }

int tEI_OSABI (stringstream& log) {
	if ( hdr->e_ident[EI_OSABI] != ELFOSABI_LINUX ) {
		log << "APPLICATION BINARY INTERFACE  MISSMATCH" << endl;
		log << hdr->e_ident[EI_OSABI] << " , " << ELFOSABI_LINUX << endl;
		return 1;
	} else {
	    return 0;
	}
}

int PAD (stringstream& log) {
    for ( int i=EI_OSABI +1; i< EI_NIDENT; i++ ) {
        if ( hdr->e_ident[i] != 0 ) {
            log << "NON-ZERO PADDING in ELF_HEADER" << endl;
            log << hdr->e_ident[i] << " , " << 0 << endl;
            return 1;
        }
    }
    return 0;
}

int te_type(stringstream& log) {
    if ( hdr->e_type != ET_REL ) {
        log << "Invalid type for object file" << endl;
        log << hdr->e_type << " , " << ET_REL  << endl;
        return 1;
    } else {
        return 0;
    }
}

int machine(stringstream& log) {
    if ( hdr->e_machine != EM_X86_64 ) {
        log << "Unexpected machine type" << endl;
        log << hdr->e_machine << " , " << EM_X86_64  << endl;
        return 1;
    } else {
        return 0;
    }
}

int HeaderSizes(stringstream& log ) {
    if ( hdr->e_shentsize != sizeof(Elf64_Shdr) ) {
        log << "Invalid section header size" << endl;
        log << hdr->e_shentsize << " , ";
        log  << sizeof(Elf64_Shdr)  << endl;
        return 1;
    } else if ( hdr->e_phentsize != sizeof(Elf64_Phdr) ){
        log << "Invalid program header size" << endl;
        log << hdr->e_phentsize << " , ";
        log  << sizeof(Elf64_Phdr)  << endl;
        return 1;
    } else {
        return 0;
    }
}
