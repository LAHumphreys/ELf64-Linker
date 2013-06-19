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

unsigned char * buf = new unsigned char[50000000];
DEFER ( delete [] buf;)
DataIO outfile(buf,5000000);
RawProgramHeader* newHeaders;
RawProgramHeader* oldHeaders;
int headers;

ElfHeaderX86_64 *poldHeader = NULL;
ElfHeaderX86_64 *pnewHeader = NULL;


int CompareHeaders(testLogger& log );
int CheckElfHeader(testLogger& log );

int main(int argc, const char *argv[])
{
    ElfFileReader f(argv[0]);
    
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();

    ElfFile file( content);
    file.WriteToFile(outfile);
    
    // read in the section data
    ElfHeaderX86_64 header(outfile);
    ElfHeaderX86_64 oldHeader(f);
    poldHeader = &oldHeader;
    pnewHeader = &header;
    
    newHeaders = new RawProgramHeader[header.ProgramHeaders()];
    oldHeaders = new RawProgramHeader[header.ProgramHeaders()];
    DEFER ( 
       delete [] newHeaders; 
       delete [] oldHeaders; 
    ) 

    BinaryReader newReader(outfile,header.ProgramHeadersStart());
    BinaryReader oldReader(f,oldHeader.ProgramHeadersStart());

    headers = oldHeader.ProgramHeaders();

    Test("Validate # of headers",CheckElfHeader).RunTest();

    for ( int i=0; i< oldHeader.ProgramHeaders(); i++ ) {
        newHeaders[i] = content.progHeaders[i]->RawHeader();
        oldReader >> oldHeaders[i];
    }

    Test("Validate program headers",(loggedTest)CompareHeaders).RunTest();

    return 0;
}

int CheckElfHeader(testLogger& log ) {
    if (   poldHeader->ProgramHeaders() 
        != pnewHeader->ProgramHeaders() )
    {
        log << "Missmatch in program header count!" << endl;
        log << poldHeader->ProgramHeaders(); 
        log << " , "; 
        log << pnewHeader->ProgramHeaders(); 
        return 1;
    }
    return 0;
}

int CompareHeaders(testLogger& log ) {
    for ( int i=0; i< headers; i++ ) {
        const RawProgramHeader& newHdr = newHeaders[i];
        const RawProgramHeader& oldHdr = oldHeaders[i];
        log << i << ": " << endl;
        log << "*********************************************************" << endl;
        log << "***        NEW                                      *****" << endl;
        log << newHdr.Describe() << endl;
        log << "*********************************************************" << endl;
        log << "***        OLD                                      *****" << endl;
        log << oldHdr.Describe() << endl;
        log << "*********************************************************" << endl;
        if (memcmp(&newHdr, &oldHdr, sizeof(RawProgramHeader) != 0)) {
            log << "Missmatch in program headers: " << i << endl;
            return 1;
        }
    }
    return 0;
}
