#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>


#include <iostream>

using namespace std;

#ifndef ElfReader_H
   #include "elfReader.h"
#endif


ElfReader::ElfReader ( const string &fname ):file(NULL) {
    OpenFile(fname);
}

void ElfReader::OpenFile ( const string &fname ) {

    // kill the old file
    if (file) {
       munmap(file,statBlock.st_size);
       file = NULL;
    }

    // Lets briefly pretend we're c programers
    stat (fname.c_str(), &statBlock);
    FILE *fh = fopen(fname.c_str(),"rb");
    file = mmap(NULL,statBlock.st_size, PROT_READ, 
                                        MAP_PRIVATE,
                                        fileno(fh),
                                        0);
    sptr = reinterpret_cast<const char *>(file);
    fclose(fh);
}

ElfReader::~ElfReader () {
    // kill the old file
    if (file) {
       munmap(file,statBlock.st_size);
    }
}

void ElfReader::ReadString(int offset, string &dest) {
    const char * str = this->sptr + offset;
    dest += str;
}
void ElfReader::Read(int offset, void *dest, size_t size) {
    memcpy(dest,sptr + offset,size);
}
