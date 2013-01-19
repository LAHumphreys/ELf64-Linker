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

ElfParser *p;
DataVector outfile(5000);
ElfHeaderX86_64 *header;
Elf64_Ehdr *hdr;

using namespace std;
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
	delete hdr;

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
