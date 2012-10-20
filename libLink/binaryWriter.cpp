#include "binaryWriter.h"
#include "binaryReader.h"
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

void BinaryWriter::Write(const BinaryReader& pos, long size) {
    for (long i = 0; i < size; i++) {
        file.Put(offset+i,(pos +i).Get());
    }
}

void BinaryWriter::Fill(long size) {
    this->file.Fill(offset,0,size);
}

void BinaryWriter::Fill(long size, unsigned char c ) {
    this->file.Fill(offset,c,size);
}

void BinaryWriter::WriteString(const BinaryReader &pos) {
    string buf = pos.ReadString();
    this->file.Write(this->offset, buf.c_str(),buf.length());
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

DataWriter::DataWriter(void *data, long size) {
    rawdata = (unsigned char *)data;
    length = size;
}
void DataWriter::Write(long offset, const void *src, long len ) {
    if ( offset + len >= this->length ) {
        throw "Invalid write to data object: not enough space";
    } else if ( offset < 0)  {
        throw "Invalid write to data object: -'ve offset";
    }else {
        memcpy(this->rawdata+offset,src,length);
    }
}

void DataWriter::Put(long offset, unsigned char c) {
    if ( offset >= this->length ) {
        throw "Invalid Put to data object: not enough space";
    }
    this->rawdata[offset] = c;
}

void DataWriter::Fill(long offset, unsigned char c, long count) {
    if ( offset + count >= this->length ) {
        throw "Invalid fill to data object: not enough space";
    } else if ( offset < 0)  {
        throw "Invalid fill to data object: -'ve offset";
    }else {
       memset(this->rawdata,c,count);
    }
}
