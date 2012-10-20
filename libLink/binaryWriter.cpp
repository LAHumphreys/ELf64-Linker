#include "binaryWriter.h"
#include <string>
#include <cstring>

using namespace std;

BinaryWriter::BinaryWriter( FileLikeWriter &f) 
: file(f)
{
    this->offset = 0;
}

BinaryWriter::BinaryWriter( FileLikeWriter &f, long offset) 
: file(f)
{
    this->offset = offset;
}

BinaryWriter::BinaryWriter(const BinaryWriter &other) 
: file(other.file)
{
    this->offset = other.Offset();
    // TODO: Convert to delegated constructor when gcc 4.7 is
    // available
}

void BinaryWriter::Write(void *src, long size) {
    file.Write(offset,src,size);
}

void BinaryWriter::Fill(long size) {
    this->file.Fill(offset,0,size);
}

void BinaryWriter::Fill(long size, unsigned char c ) {
    this->file.Fill(offset,c,size);
}

void BinaryWriter::WriteString(std::string& src) {
    this->file.Write(offset,src.c_str(), src.length());
}

void BinaryWriter::WriteString(const void *src) {
    this->file.Write(offset,src,strlen((const char *)src));
}

// Create a new BinaryWriter
BinaryWriter BinaryWriter::operator+( long additionalOffset) const 
{
    return BinaryWriter(file, offset + additionalOffset);
}

BinaryWriter BinaryWriter::operator+( const BinaryWriter& p) const
{
    return BinaryWriter(file, offset + p.Offset());
}

BinaryWriter BinaryWriter::operator-( long additionalOffset) const 
{
    return BinaryWriter(file, offset - additionalOffset);
}

BinaryWriter BinaryWriter::operator-( const BinaryWriter& p) const
{
    return BinaryWriter(file, offset - p.Offset());
}

// Reposition the pointer
BinaryWriter& BinaryWriter::operator+=( long additionalOffset) 
{
    offset += additionalOffset;
    return *this;
}

BinaryWriter& BinaryWriter::operator+=( const BinaryWriter& p) 
{
    offset += p.Offset();
    return *this;
}

BinaryWriter& BinaryWriter::operator-=( long additionalOffset) 
{
    offset -= additionalOffset;
    return *this;
}

BinaryWriter& BinaryWriter::operator-=( const BinaryWriter& p) 
{
    offset -= p.Offset();
    return *this;
}

BinaryWriter& BinaryWriter::operator=(long offset) {
    this->offset = offset;
    return *this;
}

BinaryWriter BinaryWriter::Begin() const {
    return BinaryWriter(file,0);
}

BinaryWriter BinaryWriter::Pos(long offset) const {
    return BinaryWriter(file,offset);
}

//Comparision operators
bool BinaryWriter::operator== (const BinaryWriter & other) const 
{
    return offset == other.Offset();
}
bool BinaryWriter::operator<= (const BinaryWriter & other) const 
{
    return offset <= other.Offset();
}
bool BinaryWriter::operator>= (const BinaryWriter & other) const 
{
    return offset >= other.Offset();
}
bool BinaryWriter::operator> (const BinaryWriter & other) const 
{
    return offset > other.Offset();
}
bool BinaryWriter::operator< (const BinaryWriter & other) const 
{
    return offset < other.Offset();
}
