#ifndef BINARY_READER_H
#define BINARY_READER_H
#include <string>
#include "binaryWriter.h"

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
    BinaryReader(const FileLikeReader& f, const long& offset);
    BinaryReader(const FileLikeReader& f);
    BinaryReader(const BinaryReader &other);

    // Access data
    virtual void Read(void *dest, long size) const;
    virtual void Read(BinaryWriter& dest, long size) const;
    virtual void Read(BinaryWriter&& dest, long size) const {
        Read(dest, size);
    }

    virtual void AppendString(std::string& dest) const;

    virtual void ReadLine(void *dest,long max,char delim) const;
    virtual void ReadLine(BinaryWriter& ,long ,char ) const;
    virtual void ReadLine(BinaryWriter&& w,long l,char c) const {
        ReadLine(w,l,c);
    };

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

    template <typename number>
    BinaryReader operator+(number additionalOffset) const;
    template <typename number>
    BinaryReader operator-(number additionalOffset) const;

    /* Read data and reposition the pointer */
    BinaryReader& Pull(void *,unsigned char delim);
    template<class T>
    BinaryReader& Pull(T&);

    /* Convienience operator that redirects to pull */
    inline BinaryReader& operator>>(char *dest) {
        return Pull(dest,'\0');
    }
    template<class T>
    inline BinaryReader& operator>>(T&);

    // Reposition the pointer
    virtual BinaryReader& operator+=(long additionalOffset);
    virtual BinaryReader& operator-=(long additionalOffset);

    // Note this is not allowed to change the reader, only the position
    virtual BinaryReader& operator=(long newOffset);

    virtual long Offset() const {return offset;}
    virtual operator long() const { return offset;}
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

#include "binaryReader.hpp"
#endif
