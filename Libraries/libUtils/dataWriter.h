#ifndef DATA_WRITER_H
#define DATA_WRITER_H
#include "binaryWriter.h"
#include <cstring>
class DataWriter : public virtual FileLikeWriter{
public:
    DataWriter(void *);
    // Note these can only actually be in-lined when the compiler knows 
    // the precise type, which will not be the case for almost all 
    // uses of the class.
    inline virtual void Write(long offset, const void *src, long size) {
        memcpy(this->rawdata+offset,src,size);
    }

    inline virtual void Put(long offset, unsigned char c) {
        rawdata[offset] = c;
    }

    inline virtual void Fill(long offset, unsigned char c, long count) {
        memset(rawdata + offset,c,count);
    }

    //pointless
    virtual void Flush( ) {};

	unsigned char * RawPtr() { return rawdata;}
	BinaryWriter Writer() { return BinaryWriter(*this,0);}
private:
    unsigned char * rawdata;
};
#endif
