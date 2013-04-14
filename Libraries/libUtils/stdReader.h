#ifndef STD_READER_H
#define STD_READER_H

#include <fstream>
#include <string>
#include "binaryReader.h"

using namespace std;

class StdReader: public virtual FileLikeReader {
public:
    StdReader(istream&f );
    virtual void Read(long offset, void *dest, long size) const;
    virtual void ReadString(long offset, std::string& dest)const;
    virtual unsigned char Get(long offset) const;

    virtual long Size() const;
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;
private:
    istream& file;
    long length;
};

class IFStreamReader: public ifstream, public StdReader {
public:
    IFStreamReader(const char *fname); 
    string Fname() { return fileName;}
private:
    string fileName;
};
#endif
