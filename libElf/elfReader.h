#ifndef ElfFileReader_H
#define ElfFileReader_H

#include "binaryReader.h"

using namespace std;

/**
    \class   ElfFileReader
    \brief   Read an Elf class
    \details Provides an abstraction of the process of mem-mapping
             the ELF file, and guarantees a call to munmap
*/
class ElfFileReader: public FileLikeReader {
public:
    ElfFileReader (const string &fname);
    virtual ~ElfFileReader();

    virtual void Read(long offset, void *dest, long size) const;
    virtual void ReadString(long offset, string& dest) const;
    virtual unsigned char Get(long offset) const;

    virtual long Size() const;
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;


private:
    void OpenFile(const string &fname);
    void *file;
    const char * sptr;
    long size;
};
#endif
