#include <string>
#include "binaryReader.h"

using namespace std;

BinaryReader::BinaryReader( const FileLikeObject &f) 
: file(f)
{
    this->offset = 0;
}

BinaryReader::BinaryReader( const FileLikeObject &f, long offset) 
: file(f)
{
    this->offset = offset;
}

BinaryReader::BinaryReader(const BinaryReader &other) 
: file(other.file)
{
    this->offset = other.Offset();
    // TODO: Convert to delegated constructor when gcc 4.7 is
    // available
}

// Create a new BinaryReader
BinaryReader BinaryReader::operator+( long additionalOffset) const 
{
    return BinaryReader(file, offset + additionalOffset);
}

BinaryReader BinaryReader::operator+( const BinaryReader& p) const
{
    return BinaryReader(file, offset + p.Offset());
}

BinaryReader BinaryReader::operator-( long additionalOffset) const 
{
    return BinaryReader(file, offset - additionalOffset);
}

BinaryReader BinaryReader::operator-( const BinaryReader& p) const
{
    return BinaryReader(file, offset - p.Offset());
}

// Reposition the pointer
BinaryReader& BinaryReader::operator+=( long additionalOffset) 
{
    offset += additionalOffset;
    return *this;
}

BinaryReader& BinaryReader::operator+=( const BinaryReader& p) 
{
    offset += p.Offset();
    return *this;
}

BinaryReader& BinaryReader::operator-=( long additionalOffset) 
{
    offset -= additionalOffset;
    return *this;
}

BinaryReader& BinaryReader::operator-=( const BinaryReader& p) 
{
    offset -= p.Offset();
    return *this;
}

BinaryReader& BinaryReader::operator=(long offset) {
    this->offset = offset;
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

//Comparision operators
bool BinaryReader::operator== (const BinaryReader & other) const 
{
    return offset == other.Offset();
}
bool BinaryReader::operator<= (const BinaryReader & other) const 
{
    return offset <= other.Offset();
}
bool BinaryReader::operator>= (const BinaryReader & other) const 
{
    return offset >= other.Offset();
}
bool BinaryReader::operator> (const BinaryReader & other) const 
{
    return offset > other.Offset();
}
bool BinaryReader::operator< (const BinaryReader & other) const 
{
    return offset < other.Offset();
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
