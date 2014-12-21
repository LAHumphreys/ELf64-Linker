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
#include <iomanip>
#include "logger.h"
#include <iomanip>

using namespace std;

unsigned char * buf = new unsigned char[50000000];
DEFER ( delete [] buf;)
DataIO outfile(buf,5000000);
FileLikeReader* infile;
RawProgramHeader* newHeaders;
RawProgramHeader* oldHeaders;
int headers;
string pathToBin;

ElfHeaderX86_64 *poldHeader = NULL;
ElfHeaderX86_64 *pnewHeader = NULL;


int CompareHeaders(testLogger& log );
int CheckElfHeader(testLogger& log );
int CompareData (testLogger& log );

int main(int argc, const char *argv[])
{
	pathToBin = "isYes/a.out";
    ElfFileReader f(pathToBin.c_str());
    
    infile = &f;
    ElfParser p(f);
    
    //parse the input file
    ElfContent content = p.Content();

    ElfFile file( content);
    file.WriteToFile(outfile);
    
    OFStreamWriter tmp("/tmp/output");
    file.WriteToFile(tmp);

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

    Test("Validate program headers",CompareHeaders).RunTest();
    Test("Validate segment data",CompareData).RunTest();

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

        // The position in the file is not /really/ a problem
        if ( newHdr.DataStart() != oldHdr.DataStart() ) { 
            log << "WARNING: Missmatch in DataStart for program headers: " << i << endl;
            log << "Continuing anyway" << endl;
        } 

        if ( newHdr.Alignment() != oldHdr.Alignment() ) { 
            log << "Missmatch in Alignment for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.FileSize() != oldHdr.FileSize() ) { 
            log << "Missmatch in FileSize for program headers: " << i << endl;
            SPRINT ( "old: " << hex << oldHdr.FileSize());
            SPRINT ( "new: " << hex << newHdr.FileSize());
            return 1;
        } 

        if ( newHdr.SizeInMemory() != oldHdr.SizeInMemory() ) { 
            log << "Missmatch in SizeInMemory for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.Address() != oldHdr.Address() ) { 
            log << "Missmatch in Address for program headers: " << i << endl;
            return 1;
        } 

        // Check Flags
        if ( newHdr.IsNull() != oldHdr.IsNull() ) { 
            log << "Missmatch in IsNull for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsLoadableSegment() != oldHdr.IsLoadableSegment() ) { 
            log << "Missmatch in IsLoadableSegment for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsDynamicLinking() != oldHdr.IsDynamicLinking() ) { 
            log << "Missmatch in IsDynamicLinking for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsInterpreter() != oldHdr.IsInterpreter() ) { 
            log << "Missmatch in IsInterpreter for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsNote() != oldHdr.IsNote() ) { 
            log << "Missmatch in IsNote for program headers: " << i << endl;
            return 1;
        } 

        /* Entry for the prog headers themselves! */
        if ( newHdr.IsProgramHeaders() != oldHdr.IsProgramHeaders() ) { 
            log << "Missmatch in IsProgramHeaders for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsThreadLocalStorage() != oldHdr.IsThreadLocalStorage() ) { 
            log << "Missmatch in IsThreadLocalStorage for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.GNUIsExceptionFrame() != oldHdr.GNUIsExceptionFrame() ) { 
            log << "Missmatch in GNUIsExceptionFrame for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.GNUIsExecutable() != oldHdr.GNUIsExecutable() ) { 
            log << "Missmatch in GNUIsExecutable for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.GNUIsReadOnlyAfterReloc() != oldHdr.GNUIsReadOnlyAfterReloc() ) { 
            log << "Missmatch in GNUIsReadOnlyAfterReloc for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsExecutable() != oldHdr.IsExecutable() ) { 
            log << "Missmatch in IsExecutable for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsReadable() != oldHdr.IsReadable() ) { 
            log << "Missmatch in IsReadable for program headers: " << i << endl;
            return 1;
        } 

        if ( newHdr.IsWriteable() != oldHdr.IsWriteable() ) { 
            log << "Missmatch in IsWriteable for program headers: " << i << endl;
            return 1;
        } 
    }
    return 0;
}

int CompareData (testLogger& log ) {
    for ( int i=0; i< headers; i++ ) {
        const RawProgramHeader& newHdr = newHeaders[i];
        const RawProgramHeader& oldHdr = oldHeaders[i];
        if ( oldHdr.Size() == 0 || !oldHdr.IsLoadableSegment()) {
            continue;
        }

        long ostart = oldHdr.DataStart();
        long nstart = newHdr.DataStart();
        size_t oend = oldHdr.FileSize();

        log << "Header: " << i << endl;


        if ( oldHdr.DataStart() == 0 ) {
            // We acknoweldge there may be a difference in the header file secition
            // (data-starts)
            ostart = oldHdr.DataStart() + headers*sizeof(Elf64_Phdr) + sizeof(Elf64_Ehdr);
            nstart = newHdr.DataStart() + headers*sizeof(Elf64_Phdr) + sizeof(Elf64_Ehdr);

            oend = oldHdr.FileSize() - ostart;
        }

        LOG( LOG_VERBOSE,
                            newHdr.Describe() 
                          + BinaryDescribe::Describe(
                                BinaryReader(*infile,ostart),
                                newHdr.FileSize()) 
                 )
        LOG( LOG_VERBOSE,
                            oldHdr.Describe() 
                          + BinaryDescribe::Describe(
                                BinaryReader(outfile,oldHdr.DataStart()),
                                oldHdr.FileSize()) 
                 )

        for (size_t j = 0; j < oend; j++) {
            if  (    outfile.Get(j+nstart) != infile->Get(j+ostart) )
            {
                log << hex << "Missmatch at index: " << j << "(" << hex << j << ")" << "!" << endl;
                log << hex << "Raw file offset: " << ostart + j;
                log << ", " << nstart + j <<  endl;
                log << "Total size of this segment: " << hex << oldHdr.FileSize() << endl;
                log << "Diff: " << infile->Get(j+ostart) << " -> " << outfile.Get(j+nstart) << endl;
                log << "Diff: " << (short) infile->Get(j+ostart) << " -> " << (short) outfile.Get(j+nstart) << endl;
                if ( j < 100 ) {
                    log << "Old Data: " << endl;
                    log << newHdr.Describe() << endl;
                    log << BinaryDescribe::Describe(
                               BinaryReader(*infile,ostart),
                               j);
                    log << "New Data: " << endl;
                    log << newHdr.Describe() << endl;
                    log << BinaryDescribe::Describe(
                               BinaryReader(outfile,nstart),
                               j);
                } else {
                    log << "Old Data: " << endl;
                    log << newHdr.Describe() << endl;
                    log << BinaryDescribe::Describe(
                               BinaryReader(*infile,ostart+j-100),
                               105);
                    log << "New Data: " << endl;
                    log << newHdr.Describe() << endl;
                    log << BinaryDescribe::Describe(
                               BinaryReader(outfile,nstart+j-100),
                               105);

                }
                return 1;
            }
        }
    }
    return 0;
}
