#include <string>
using namespace std;
#ifndef BINARY_WRITER_H
#define BINARY_WRITER_H

class BinaryReader;

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
    virtual BinaryWriter operator+ (long additionalOffset) const;
    virtual BinaryWriter operator+ (const BinaryWriter& p) const;
    virtual BinaryWriter operator- (long additionalOffset) const;
    virtual BinaryWriter operator- (const BinaryWriter& p) const;
    virtual BinaryWriter NextBoundrary (long alignment) const;

    virtual long operator%(long alignment) const;

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
    virtual operator long() { return offset;}
private: 
    FileLikeWriter& file;
    long offset;
};

class DataWriter : FileLikeWriter{
public:
    DataWriter(void *, long );
    virtual void Write(long offset, const void *src, long size);
    virtual void Put(long offset, unsigned char c);
    virtual void Fill(long offset, unsigned char c, long count);
    //pointless
    virtual void Flush( ) {};
private:
    unsigned char * rawdata;
    long length;
};

template<int size>
class DataLump: DataWriter
{
public:
     DataLump(): DataWriter(this->data,size){};
private:
     unsigned char rawData[size];
};
#endif
