#ifndef DATA_READER_H
#define DATA_READER_H
#include "binaryReader.h"
#include <cstring>

class DataReader : public virtual FileLikeReader{
public:
    DataReader(void *, long len);

    inline virtual void Read(long offset, void *dest, long size) const {
        memcpy(dest,rawData+offset,size);
    }

    inline virtual void ReadString(long offset, std::string& dest) const {
        dest = reinterpret_cast<const char *>(rawData+offset);             
    }

    inline virtual unsigned char Get(long offset) const {
        return  rawData[offset];
    }

    inline virtual long Size() const {return len;} 
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;

	const unsigned char * ReadPtr() { return rawData;}
	BinaryReader Reader() { return BinaryReader (*this,0);}
private:
    const unsigned char * rawData;
    long len;
};
#endif
