#ifndef ElfReader_H
#define ElfReader_H

#include <string>
#include <sys/stat.h>
using namespace std;

/**
    \class   ElfReader
    \brief   Read an Elf class
    \details Provides an abstraction of the process of mem-mapping
             the ELF file, and guarantees a call to munmap
*/
class ElfReader {
public:
    ElfReader (const string &fname);
    void OpenFile(const string &fname);
    void Read(int offset, void *dest, size_t size);
    void ReadString(int offset, string& dest);
    virtual ~ElfReader();



private:
   void *file;
   const char * sptr;
   struct stat statBlock;
};
#endif
