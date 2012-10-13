#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "elfReader.h"

using namespace std;



ElfFileReader::ElfFileReader ( const string &fname ):file(NULL) {
    OpenFile(fname);
}

void ElfFileReader::OpenFile ( const string &fname ) {

    // kill the old file
    if (file) {
       munmap(file,size);
       file = NULL;
       this->size = 0;
    }

    // Lets briefly pretend we're c programers
    struct stat statBlock;
    stat (fname.c_str(), &statBlock);
    size=statBlock.st_size;

    FILE *fh = fopen(fname.c_str(),"rb");
    file = mmap(NULL,size, PROT_READ, MAP_PRIVATE, fileno(fh), 0);
    sptr = reinterpret_cast<const char *>(file);
    fclose(fh);
}

ElfFileReader::~ElfFileReader () {
    // kill the old file
    if (file) {
       munmap(file,size);
    }
}

void ElfFileReader::ReadString(long offset, string &dest) const {
    const char * str = this->sptr + offset;
    dest=str;
}
void ElfFileReader::Read(long offset, void *dest, long size) const {
    memcpy(dest,sptr + offset,size);
}

SimpleBinaryPosition ElfFileReader::Begin() const {
    return SimpleBinaryPosition(*this,0);
}
SimpleBinaryPosition ElfFileReader::End() const {
    return SimpleBinaryPosition(*this,size);
}

SimpleBinaryPosition ElfFileReader::Pos(long offset) const {
    return SimpleBinaryPosition(*this,offset) ;
}
