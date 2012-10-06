#ifndef ElfFileReader_H
#define ElfFileReader_H

#include <sys/stat.h>
#include "binaryReader.h"


using namespace std;

/**
    \class   ElfFileReader
    \brief   Read an Elf class
    \details Provides an abstraction of the process of mem-mapping
             the ELF file, and guarantees a call to munmap
*/
class ElfFileReader: public BinaryReader {
public:
    ElfFileReader (const string &fname);
    virtual ~ElfFileReader();

    virtual void Read(long offset, void *dest, long size) const;
    virtual void ReadString(long offset, string& dest) const;

    typedef SimpleBinaryPosition PositionType;

    virtual PositionType Begin() const;
    virtual PositionType Pos(long offset) const;

    string fname;
    private:
    void OpenFile(const string &fname);
    void *file;
    const char * sptr;
    struct stat statBlock;
};
#endif
