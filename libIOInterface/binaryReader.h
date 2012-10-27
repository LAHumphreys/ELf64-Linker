#include <string>
#include "binaryWriter.h"
#ifndef BINARY_READER_H
#define BINARY_READER_H

using namespace std;

class BinaryReader; //forward declaration
class BinaryReader;

// Requirements for a class to be wrapped by BinaryReader
class FileLikeReader {
public:
    virtual void Read(long offset, void *dest, long size) const =0;
    virtual void ReadString(long offset, std::string& dest)const =0;
    virtual unsigned char Get(long offset) const =0;

    virtual long Size() const =0;
    virtual long Next( long offset, unsigned char c) const =0;
    virtual long Last( long offset, unsigned char c) const =0;
};

class BinaryReader {
public:
    BinaryReader(const FileLikeReader& f, long offset);
    BinaryReader(const FileLikeReader& f);
    BinaryReader(const BinaryReader &other);

    // Access data
    virtual void Read(void *dest, long size) const;
    virtual void Read(BinaryWriter& dest, long size) const;
    virtual void AppendString(std::string& dest) const;

    virtual void ReadLine(void *dest,long max,char delim) const;
    virtual void ReadLine(BinaryWriter& ,long ,char ) const;

    virtual void ReadString(std::string& dest) const;
    virtual std::string ReadString() const;

    virtual unsigned char * Dup(long size) const;

    unsigned char Get() const;

    // Index file
    virtual BinaryReader Begin() const;
    virtual BinaryReader End() const;
    virtual BinaryReader Pos(long offset) const;
    virtual BinaryReader Find(unsigned char c) const;
    virtual BinaryReader RFind(unsigned char c) const;

    // Create a new BinaryReader
    virtual BinaryReader operator+ (long additionalOffset) const;
    virtual BinaryReader operator+ (const BinaryReader& p) const;
    virtual BinaryReader operator- (long additionalOffset) const;
    virtual BinaryReader operator- (const BinaryReader& p) const;

    // Reposition the pointer
    virtual BinaryReader& operator+=(long additionalOffset);
    virtual BinaryReader& operator+=(const BinaryReader&);
    virtual BinaryReader& operator-=(long additionalOffset);
    virtual BinaryReader& operator-=(const BinaryReader&);

    virtual BinaryReader& operator=(long offset);

    // Comparision of offset
    virtual bool operator <(const BinaryReader& other) const;
    virtual bool operator >(const BinaryReader& other) const;
    virtual bool operator <=(const BinaryReader& other) const;
    virtual bool operator >=(const BinaryReader& other) const;
    virtual bool operator ==(const BinaryReader& other) const;

    virtual long Offset() const {return offset;}
    virtual operator long() { return offset;}
private: 
    const FileLikeReader& file;
    long offset;
};

class SubReader: public FileLikeReader {
public:
    SubReader(const BinaryReader &p, long size);

    // Implement the interface for FileLikeReader
    virtual void Read(long offset, void *dest, long size) const ;
    virtual void ReadString(long offset, std::string& dest)const;
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;
    virtual long Size() const;
    virtual unsigned char Get(long offset) const;

    // Utility functions not required by the interface
    virtual BinaryReader Begin() const;
    virtual BinaryReader End() const;
    virtual BinaryReader Pos(long offset) const;
private:
    BinaryReader pos_;
    long size;
};
#endif
