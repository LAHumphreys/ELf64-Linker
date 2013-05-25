#ifndef BINARY_WRITER_H
#define BINARY_WRITER_H

#include <string>
class BinaryReader;
class BinaryWriter;
#include "binaryReader.h"
using namespace std;


class FileLikeWriter {
public:
    virtual void Write(long offset, const void *src, long size)  =0;
    virtual void Put(long offset, unsigned char c)  =0;
    virtual void Fill(long offset, unsigned char c, long count)  =0;
    virtual void Flush( ) =0;
};

class BinaryWriter {
public:
    BinaryWriter(FileLikeWriter& f, long offset);
    BinaryWriter(FileLikeWriter& f);
    BinaryWriter(const BinaryWriter &other);

    // Write data
    virtual void Write(const void *src, long size);
    virtual void Write(const BinaryReader& pos, long size);
    virtual void WriteString(const std::string& src);
    virtual void WriteString(const void *src);
    virtual void WriteString(const BinaryReader &pos);
    virtual void Fill(long size);
    virtual void Fill(long size, unsigned char c);
    virtual void FillTo ( const BinaryWriter &stop, 
                          unsigned char c = '\0');

    // Index file
    virtual BinaryWriter Begin() const;
    virtual BinaryWriter Pos(long offset) const;

    // Create a new BinaryWriter
    template<typename number>
    BinaryWriter operator+ (number additionalOffset) const;
    template<typename number>
    BinaryWriter operator- (number additionalOffset) const;
    virtual BinaryWriter NextBoundrary (long alignment) const;

    virtual long operator%(long alignment) const;

    // Reposition the pointer
    virtual BinaryWriter& operator+=(long additionalOffset);
    virtual BinaryWriter& operator-=(long additionalOffset);

    virtual BinaryWriter& operator=(long offset);

    /* Write data and reposition the pointer */
    BinaryWriter& Push(const char *,char delim);
    template<class T>
    BinaryWriter& Push(T&);

    /* Convienience operator that redirects to push */
    inline BinaryWriter& operator<<(const char *src) {
        return Push(src,'\0');
    }

    template<class T>
    inline BinaryWriter& operator<<(const T&);

    virtual long& Offset() {return offset;}
    virtual long Offset() const {return offset;}
    virtual operator long() const { return offset;}
private: 
    FileLikeWriter& file;
    long offset;
};
#include "binaryWriter.hpp"
#endif
