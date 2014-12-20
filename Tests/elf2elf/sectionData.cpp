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
#include "binaryDescribe.h"
#include <string>

using namespace std;

const long MEG=1024*1024;
DataLump<20*MEG>* outfile;
FileLikeReader* infile;
Elf64_Shdr* newSections;
Elf64_Shdr* oldSections;
string* names;
int sections;


int CompareSections(testLogger& log );

int main(int argc, const char *argv[])
{
    ElfFileReader f(argv[0]);
    
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();

    ElfFile file( content);
    outfile = new DataLump<20*MEG>();
    file.WriteToFile(*outfile);
    
    // read in the section data
    ElfHeaderX86_64 header(*outfile);
    ElfHeaderX86_64 oldHeader(f);
    
    newSections = new Elf64_Shdr[header.Sections()];
    oldSections = new Elf64_Shdr[header.Sections()];
    names = new string[header.Sections()];
    DEFER ( 
       delete [] newSections; 
       delete [] oldSections; 
       delete [] names;
    ) 

    infile = &f;

    BinaryReader newReader(*outfile,header.SectionTableStart());
    BinaryReader oldReader(f,oldHeader.SectionTableStart());

    for ( int i=0; i< header.Sections(); i++ ) {
        newReader >> newSections[i];
        oldReader >> oldSections[i];
        names[i] = p.Content().sections[i]->Name();
    }

    sections = header.Sections();

    Test("Validate section data",(loggedTest)CompareSections).RunTest();
    return 0;
}

int CompareSections(testLogger& log ) {
    for ( int i=0; i< sections; i++ ) {
        const SectionHeader& newHdr = newSections[i];
        const SectionHeader& oldHdr = oldSections[i];

        if ( names[i] == ".shstrtab" ) {
            // we re-build this ourselves
            continue;
        }

        if ( !oldHdr.HasFileData())
        {
            // Nothing to compare...
            continue;
        }

        log << "Section: " << i << endl;

        log << "Old Data: " << endl;
        log << BinaryDescribe::Describe( 
                   BinaryReader(*infile,newHdr.DataStart()),
                   100);
        log << "New Data: " << endl;
        log << BinaryDescribe::Describe( 
                   BinaryReader(*outfile,oldHdr.DataStart()),
                   100);

        for (size_t j = 0; j < oldHdr.DataSize(); j++) {
            if  (    outfile->Get(j+newHdr.DataStart())
                  != infile->Get(j+oldHdr.DataStart()) ) 
            {
            	long oldIdx = j+oldHdr.DataStart();
            	long newIdx = j+newHdr.DataStart();

                log << "Missmatch at index: " << j << "!" << endl;
                log << oldIdx << ": " << infile->Get(oldIdx) << " -> "
                    << newIdx << ": " << outfile->Get(newIdx) << endl;
                return 1;
            }
        }
    }
    return 0;
}
