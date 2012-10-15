#include <string>
#ifndef BINARY_READER_H
#define BINARY_READER_H

using namespace std;

class BinaryReader; //forward declaration
class BinaryReader;

// Requirements for a class to be wrapped by BinaryReader
class FileLikeObject {
public:
    virtual void Read(long offset, void *dest, long size) const =0;
    virtual void ReadString(long offset, std::string& dest)const =0;
    virtual long Size() const =0;
    virtual long Next( long offset, unsigned char c) const =0;
    virtual long Last( long offset, unsigned char c) const =0;
};

class BinaryReader {
public:
    BinaryReader(const FileLikeObject& f, long offset);
    BinaryReader(const FileLikeObject& f);
    BinaryReader(const BinaryReader &other);

    // Access data
    virtual void Read(void *dest, long size) const;
    virtual unsigned char * Dup(long size) const;
    virtual void ReadString(std::string& dest) const;
    virtual std::string ReadString() const;
    virtual void AppendString(std::string& dest) const;
    virtual void ReadLine(void *dest,long max,char delim) const;

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
private: 
    const FileLikeObject& file;
    long offset;
};

class SubReader: public FileLikeObject {
public:
    SubReader(const BinaryReader &p, long size);

    // Implement the interface for FileLikeObject
    virtual void Read(long offset, void *dest, long size) const ;
    virtual void ReadString(long offset, std::string& dest)const;
    virtual long Next( long offset, unsigned char c) const;
    virtual long Last( long offset, unsigned char c) const;
    virtual long Size() const;

    // Utility functions not required by the interface
    virtual BinaryReader Begin() const;
    virtual BinaryReader End() const;
    virtual BinaryReader Pos(long offset) const;
private:
    BinaryReader pos_;
    long size;
};
#endif
