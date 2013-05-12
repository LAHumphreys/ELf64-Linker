#include <string>
#include "binaryReader.h"

using namespace std;

BinaryReader::BinaryReader( const FileLikeReader &f) 
: file(f), offset(0) { }

BinaryReader::BinaryReader( const FileLikeReader &f, const long& offset) 
: file(f), offset(offset) { }

BinaryReader::BinaryReader(const BinaryReader &other) 
: file(other.file), offset(other.Offset()) {}


// Reposition the pointer
BinaryReader& BinaryReader::operator+=( long additionalOffset) 
{
    offset += additionalOffset;
    return *this;
}

BinaryReader& BinaryReader::operator-=( long additionalOffset) 
{
    offset -= additionalOffset;
    return *this;
}

BinaryReader& BinaryReader::operator=(long newOffset) {
    this->offset = newOffset;
    return *this;
}

BinaryReader BinaryReader::Begin() const {
    return BinaryReader(file,0);
}

BinaryReader BinaryReader::End() const {
    return BinaryReader(file,file.Size());
}

BinaryReader BinaryReader::Pos(long offset) const {
    return BinaryReader(file,offset);
}

BinaryReader BinaryReader::Find(unsigned char c) const {
    BinaryReader loc(file,file.Next(offset, c));
    if ( loc < Begin() )
        return Begin();
    else if ( loc > End() )
        return End();
    else
        return loc;
}

BinaryReader BinaryReader::RFind(unsigned char c) const {
    BinaryReader loc(file,file.Last(offset, c));
    if ( loc < Begin() )
        return Begin();
    else if ( loc > End() )
        return End();
    else
        return loc;
}

void BinaryReader::Read(void *dest, long size) const{
    file.Read(offset,dest,size);
}

void BinaryReader::Read(BinaryWriter& pos, long size) const {
    pos.Write(*this,size);
}

unsigned char * BinaryReader::Dup(long size) const {
    unsigned char * data = new unsigned char[size];
    file.Read(offset,data,size);
    return data;
}

void BinaryReader::ReadLine(void *dest, long max, char delim) const{
    long loc = file.Next(offset, delim);
    if ( loc - offset > max ) {
        file.Read(offset,dest,max);
    } else {
        file.Read(offset,dest,loc - offset);
    }
}

void BinaryReader::ReadLine(BinaryWriter& pos, long maxsize,
                                                     char delim ) 
                                                     const
{
    long loc = file.Next(offset, delim);
    if ( loc - offset > maxsize ) {
        pos.Write(*this,maxsize);
    } else {
        pos.Write(*this,loc-offset);
    }
}

void BinaryReader::ReadString(string& dest) const{
    file.ReadString(offset,dest);
}

void BinaryReader::AppendString(string& dest) const{
    string s;
    file.ReadString(offset,s);
    dest+=s;
}

string BinaryReader::ReadString() const {
    string s;
    file.ReadString(offset,s);
    return s;
}

unsigned char BinaryReader::Get() const {
    return this->file.Get(this->offset);
}

SubReader::SubReader(const BinaryReader &p, long size)
    :pos_(p) 
{
    this->size = size;
}

void SubReader::Read(long offset, void *dest, long size) const {
    (pos_ + offset).Read(dest,size);
}

void SubReader::ReadString(long offset, std::string& dest)const {
    (pos_ + offset).ReadString(dest);
}

unsigned char SubReader::Get(long offset)const {
    (pos_ + offset).Get();
}

long SubReader::Size() const {
    return size;
}

long SubReader::Next( long offset, unsigned char c) const
{
    BinaryReader loc = (pos_ + offset).Find(c);
    if ( loc > pos_ + size )
        return Size();
    else
        return  (loc - pos_).Offset();
}

long SubReader::Last( long offset, unsigned char c) const
{
    BinaryReader loc = (pos_ + offset).RFind(c);
    if ( loc < pos_)
        return 0;
    else
        return loc.Offset();
}

// Extra utility functions
BinaryReader SubReader::Begin() const {
    return BinaryReader(*this,0);
}
BinaryReader SubReader::End() const{
    return BinaryReader(*this,size);
}
BinaryReader SubReader::Pos(long offset) const {
    return BinaryReader(*this,offset);
}

BinaryReader& BinaryReader::Pull(void* dest, unsigned char delim) {
    long end = file.Next(offset,delim);
    long len;
    if ( end >= file.Size() ) {
        len = file.Size()-offset;
    } else {
        len = end + 1 - offset; /*remember to +1 for \0*/
    }
    Read(dest,len);
    offset+=len;
    return *this;
}


