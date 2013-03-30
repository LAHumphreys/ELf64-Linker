#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

#include <vector>
#include "fileLikeObject.h"

using namespace std;

class DataVector: public vector<unsigned char>,
                  public FileLikeObject {
public:
    DataVector(long size);
    //Writer functions
    virtual void Write(long offset, const void *src, long size);
    virtual void Put(long offset, unsigned char c);
    virtual void Fill(long offset, unsigned char c, long count);
    virtual void Flush( ){}; //pointless
    virtual BinaryWriter Writer() { return BinaryWriter(*this,0);}

    //Reader Functions
    virtual void Read(long offset, void *dest, long size) const;
    virtual void ReadString(long offset, std::string& dest)const;
    virtual long Size() const;
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;
    virtual unsigned char Get(long offset) const;
    virtual BinaryReader Reader() {return BinaryReader(*this,0);}
};

#endif
