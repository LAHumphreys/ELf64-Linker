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
#include "util_time.h"
#include <string>

using namespace std;

DataLump<5000> outfile;
Elf64_Shdr* newSections;
Elf64_Shdr* oldSections;
string* names;
ElfContent* p_content=NULL;
int sections;


int CompareContent(testLogger& log );
int CompareSections(testLogger& log );
int Init(testLogger& log );

int main(int argc, const char *argv[])
{
    // So that gprof can analyse the file building
    Test("Initialising the data",(loggedTest)Init).RunTest();

    ElfFileReader f(argv[0]);
    
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();
    p_content = &content;

    ElfFile file( content);
    
    // read in the section data
    ElfHeaderX86_64 header(outfile);
    ElfHeaderX86_64 oldHeader(f);
    
    newSections = new Elf64_Shdr[header.Sections()];
    oldSections = new Elf64_Shdr[header.Sections()];
    names = new string[header.Sections()];
    DEFER ( 
       delete [] newSections; 
       delete [] oldSections; 
       delete [] names;
    ) 

    BinaryReader newReader(outfile,header.SectionTableStart());
    BinaryReader oldReader(f,oldHeader.SectionTableStart());

    for ( int i=0; i< header.Sections(); i++ ) {
        newReader >> newSections[i];
        oldReader >> oldSections[i];
        names[i] = p.Content().sections[i]->Name();
        
    }

    sections = header.Sections();

    Test("Validate parsed headers",(loggedTest)CompareContent).RunTest();
    Test("Validate generated headers",(loggedTest)CompareSections).RunTest();
    return 0;
}

int Init(testLogger& log ) {
    /*
     * Tests to validate the string table written by ElfFile
     */
    ElfFileReader f("../elf2elf/isYes/isYes.o");
    
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();

    ElfFile file( content);
    
    // Write out the data
    outfile.Fill(0,'\0',5000);
    file.WriteToFile(outfile.Writer());

    // read in the section data
    BinaryReader readPos(outfile);
    ElfHeaderX86_64 header(readPos);

    newSections = new Elf64_Shdr[header.Sections()];
    oldSections = new Elf64_Shdr[header.Sections()];
    DEFER ( 
       delete [] newSections; 
       delete [] oldSections; 
    ) 
    

    BinaryReader newReader(outfile,header.SectionTableStart());

    for ( int i=0; i< header.Sections(); i++ ) {
        newReader >> newSections[i];
        oldSections[i] = *(content.sections[i]);
    }

    sections = header.Sections();
    return 0;
}

int CompareContent(testLogger& log ) {
    for ( int i=0; i< sections; i++ ) {
        const SectionHeader& oldHdr = oldSections[i];
        const SectionHeader& newHdr = *(p_content->sections[i]);
        log << i << ": " << names[i] << endl;
        log << "*********************************************************" << endl;
        log << "***        NEW                                      *****" << endl;
        log << newHdr.Descripe() << endl;
        log << "*********************************************************" << endl;
        log << "***        OLD                                      *****" << endl;
        log << oldHdr.Descripe() << endl;
        log << "*********************************************************" << endl;

        if ( newHdr.Writeable() != oldHdr.Writeable() ) {
            log << "Writeable missmatch: ";
            log << newHdr.Writeable() << " , ";
            log  << oldHdr.Writeable() << endl;
            return 1;
        }

        if ( newHdr.Allocate() != oldHdr.Allocate() ) {
            log << "Allocate missmatch: ";
            log << newHdr.Allocate() << " , ";
            log  << oldHdr.Allocate() << endl;
            return 1;
        }

        if ( newHdr.Executable() != oldHdr.Executable() ) {
            log << "Executable missmatch: ";
            log << newHdr.Executable() << " , ";
            log  << oldHdr.Executable() << endl;
            return 1;
        }

        if ( newHdr.IsSymTable() != oldHdr.IsSymTable() ) {
            log << "IsSymTable missmatch: ";
            log << newHdr.IsSymTable() << " , ";
            log  << oldHdr.IsSymTable() << endl;
            return 1;
        }

        if ( newHdr.IsStringTable() != oldHdr.IsStringTable() ) {
            log << "IsStringTable missmatch: ";
            log << newHdr.IsStringTable() << " , ";
            log  << oldHdr.IsStringTable() << endl;
            return 1;
        }

        if ( newHdr.IsRelocTable() != oldHdr.IsRelocTable() ) {
            log << "IsRelocTable missmatch: ";
            log << newHdr.IsRelocTable() << " , ";
            log  << oldHdr.IsRelocTable() << endl;
            return 1;
        }

        if ( newHdr.Alignment() != oldHdr.Alignment() ) {
            log << "Alignment missmatch: ";
            log << newHdr.Alignment() << " , ";
            log  << oldHdr.Alignment() << endl;
            return 1;
        }

        if ( newHdr.DataSize() != oldHdr.DataSize() ) {
            if ( oldHdr.IsStringTable() ) {
                log << "Ignoring DataSize missmatch in string table ";
                log << newHdr.DataSize() << " , ";
                log  << oldHdr.DataSize() << endl;
            }
            else {
                log << "DataSize missmatch: ";
                log << newHdr.DataSize() << " , ";
                log  << oldHdr.DataSize() << endl;
                return 1;
            }
        }

        if ( newHdr.ItemSize() != oldHdr.ItemSize() ) {
            log << "ItemSize missmatch: ";
            log << newHdr.ItemSize() << " , ";
            log  << oldHdr.ItemSize() << endl;
            return 1;
        }

        if ( newHdr.Address() != oldHdr.Address() ) {
            log << "Address missmatch: ";
            log << newHdr.Address() << " , ";
            log  << oldHdr.Address() << endl;
            return 1;
        }
    }
    return 0;
}

int CompareSections(testLogger& log ) {
    for ( int i=0; i< sections; i++ ) {
        const SectionHeader& newHdr = newSections[i];
        const SectionHeader& oldHdr = oldSections[i];
        log << i << ": " << names[i] << endl;
        log << "*********************************************************" << endl;
        log << "***        NEW                                      *****" << endl;
        log << newHdr.Descripe() << endl;
        log << "*********************************************************" << endl;
        log << "***        OLD                                      *****" << endl;
        log << oldHdr.Descripe() << endl;
        log << "*********************************************************" << endl;

        if ( newHdr.Writeable() != oldHdr.Writeable() ) {
            log << "Writeable missmatch: ";
            log << newHdr.Writeable() << " , ";
            log  << oldHdr.Writeable() << endl;
            return 1;
        }

        if ( newHdr.Allocate() != oldHdr.Allocate() ) {
            log << "Allocate missmatch: ";
            log << newHdr.Allocate() << " , ";
            log  << oldHdr.Allocate() << endl;
            return 1;
        }

        if ( newHdr.Executable() != oldHdr.Executable() ) {
            log << "Executable missmatch: ";
            log << newHdr.Executable() << " , ";
            log  << oldHdr.Executable() << endl;
            return 1;
        }

        if ( newHdr.IsSymTable() != oldHdr.IsSymTable() ) {
            log << "IsSymTable missmatch: ";
            log << newHdr.IsSymTable() << " , ";
            log  << oldHdr.IsSymTable() << endl;
            return 1;
        }

        if ( newHdr.IsStringTable() != oldHdr.IsStringTable() ) {
            log << "IsStringTable missmatch: ";
            log << newHdr.IsStringTable() << " , ";
            log  << oldHdr.IsStringTable() << endl;
            return 1;
        }

        if ( newHdr.IsRelocTable() != oldHdr.IsRelocTable() ) {
            log << "IsRelocTable missmatch: ";
            log << newHdr.IsRelocTable() << " , ";
            log  << oldHdr.IsRelocTable() << endl;
            return 1;
        }

        if ( newHdr.Alignment() != oldHdr.Alignment() ) {
            log << "Alignment missmatch: ";
            log << newHdr.Alignment() << " , ";
            log  << oldHdr.Alignment() << endl;
            return 1;
        }

        if ( newHdr.DataStart() != oldHdr.DataStart() ) {
            bool fail = true;
            if (   oldHdr.IsRelocTable() 
                || oldHdr.IsStringTable() 
                || oldHdr.IsSymTable()) 
            {
                log << "Our builder doens't put any padding between the end of data";
                log << " and the start of the special sections" << endl;
                log << " Ignoring data-start missmatch: ";
                log << newHdr.DataStart() << " , ";
                log  << oldHdr.DataStart() << endl;
                fail = false;
            }
            if (fail) { 
                log << "DataStart() missmatch: ";
                log << newHdr.DataStart() << " , ";
                log  << oldHdr.DataStart();
                return 1;
            }
        }

        if ( newHdr.DataSize() != oldHdr.DataSize() ) {
            if ( oldHdr.IsStringTable() ) {
                log << "Ignoring DataSize missmatch in string table ";
                log << newHdr.DataSize() << " , ";
                log  << oldHdr.DataSize() << endl;
            }
            else {
                log << "DataSize missmatch: ";
                log << newHdr.DataSize() << " , ";
                log  << oldHdr.DataSize() << endl;
                return 1;
            }
        }

        if ( newHdr.ItemSize() != oldHdr.ItemSize() ) {
            log << "ItemSize missmatch: ";
            log << newHdr.ItemSize() << " , ";
            log  << oldHdr.ItemSize() << endl;
            return 1;
        }

        if ( newHdr.Address() != oldHdr.Address() ) {
            log << "Address missmatch: ";
            log << newHdr.Address() << " , ";
            log  << oldHdr.Address() << endl;
            return 1;
        }
    }
    return 0;
}
