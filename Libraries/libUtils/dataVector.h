#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

#include <vector>
#include "fileLikeObject.h"

using namespace std;

class DataVector: private vector<unsigned char>,
                  public FileLikeObject {
public:
    DataVector(long size);
    //Writer functions
    virtual void Write(long offset, const void *src, long size);
    virtual void Put(long offset, unsigned char c);
    virtual void Fill(long offset, unsigned char c, long count);
    virtual void Flush( ){}; //pointless
    virtual BinaryWriter Writer() { return BinaryWriter(*this,0);}

    // Reader Functions
    virtual void Read(long offset, void *dest, long size) const;
    virtual void ReadString(long offset, std::string& dest)const;
    virtual long Size() const;
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;
    virtual unsigned char Get(long offset) const;
    virtual BinaryReader Reader() {return BinaryReader(*this,0);}

    // Utility Functions
    virtual void Resize(long size);
    virtual const unsigned char* RawData() { return this->data();}

    unsigned char& operator[](long idx) { 
        return vector<unsigned char>::operator[](idx);
    }

    const unsigned char& operator[](long idx) const { 
        return vector<unsigned char>::operator[](idx);
    }


    // Expose the iterators
    using vector<unsigned char>::begin;
    using vector<unsigned char>::end;
};

#endif
