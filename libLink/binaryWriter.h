#include <string>
using namespace std;
#ifndef BINARY_WRITER_H
#define BINARY_WRITER_H

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

    // Access data
    virtual void Write(void *src, long size);
    virtual void WriteString(std::string& src);
    virtual void WriteString(const void *src);
    virtual void Fill(long size);
    virtual void Fill(long size, unsigned char c);

    // Index file
    virtual BinaryWriter Begin() const;
    virtual BinaryWriter Pos(long offset) const;

    // Create a new BinaryWriter
    virtual BinaryWriter operator+ (long additionalOffset) const;
    virtual BinaryWriter operator+ (const BinaryWriter& p) const;
    virtual BinaryWriter operator- (long additionalOffset) const;
    virtual BinaryWriter operator- (const BinaryWriter& p) const;

    // Reposition the pointer
    virtual BinaryWriter& operator+=(long additionalOffset);
    virtual BinaryWriter& operator+=(const BinaryWriter&);
    virtual BinaryWriter& operator-=(long additionalOffset);
    virtual BinaryWriter& operator-=(const BinaryWriter&);

    virtual BinaryWriter& operator=(long offset);

    // Comparision of offset
    virtual bool operator <(const BinaryWriter& other) const;
    virtual bool operator >(const BinaryWriter& other) const;
    virtual bool operator <=(const BinaryWriter& other) const;
    virtual bool operator >=(const BinaryWriter& other) const;
    virtual bool operator ==(const BinaryWriter& other) const;

    virtual long Offset() const {return offset;}
private: 
    FileLikeWriter& file;
    long offset;
};
#endif
