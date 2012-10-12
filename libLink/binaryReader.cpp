#include <string>
#include "binaryReader.h"

using namespace std;

SimpleBinaryPosition::SimpleBinaryPosition(
                                const BinaryReader &r,
                                long offset) 
: reader(r)
{
    this->offset = offset;
}

SimpleBinaryPosition::SimpleBinaryPosition(
                                const BinaryPosition &other) 
: reader(other.Reader())
{
    this->offset = other.Offset();
    // TODO: Convert to delegated constructor when gcc 4.7 is
    // available
}

SimpleBinaryPosition::SimpleBinaryPosition(
                                BinaryPosition &&other) 
: reader(other.Reader())
{
    this->offset = other.Offset();
    // TODO: Convert to delegated constructor when gcc 4.7 is
    // available
}
 
// Create a new BinaryPosition
SimpleBinaryPosition SimpleBinaryPosition::operator+(
                                 long additionalOffset) const 
{
    return SimpleBinaryPosition(reader, offset + additionalOffset);
}

SimpleBinaryPosition SimpleBinaryPosition::operator+(
                                    const BinaryPosition& p) const
{
    return SimpleBinaryPosition(reader, offset + p.Offset());
}

SimpleBinaryPosition SimpleBinaryPosition::operator-(
                                 long additionalOffset) const 
{
    return SimpleBinaryPosition(reader, offset - additionalOffset);
}

SimpleBinaryPosition SimpleBinaryPosition::operator-(
                                    const BinaryPosition& p) const
{
    return SimpleBinaryPosition(reader, offset - p.Offset());
}

// Reposition the pointer
SimpleBinaryPosition& SimpleBinaryPosition::operator+=(
                                    long additionalOffset) 
{
    offset += additionalOffset;
    return *this;
}

SimpleBinaryPosition& SimpleBinaryPosition::operator+=(
                                    const BinaryPosition& p) 
{
    offset += p.Offset();
    return *this;
}

SimpleBinaryPosition& SimpleBinaryPosition::operator-=(
                                    long additionalOffset) 
{
    offset -= additionalOffset;
    return *this;
}

SimpleBinaryPosition& SimpleBinaryPosition::operator-=(
                                    const BinaryPosition& p) 
{
    offset -= p.Offset();
    return *this;
}

SimpleBinaryPosition& SimpleBinaryPosition::operator=(long offset) {
    this->offset = offset;
    return *this;
}

void SimpleBinaryPosition::Read(void *dest, long size) const{
    reader.Read(offset,dest,size);
}

unsigned char * SimpleBinaryPosition::Dup(long size) const {
    unsigned char * data = new unsigned char[size];
    reader.Read(offset,data,size);
    return data;
}

void SimpleBinaryPosition::ReadString(string& dest) const{
    reader.ReadString(offset,dest);
}

void SimpleBinaryPosition::AppendString(string& dest) const{
    string s;
    reader.ReadString(offset,s);
    dest+=s;
}

string SimpleBinaryPosition::ReadString() const {
    string s;
    reader.ReadString(offset,s);
    return s;
}

SubReader::SubReader(const BinaryPosition &p) :pos_(p) { }

void SubReader::Read(long offset, void *dest, long size) const {
    pos_.Reader().Read( pos_.Offset()+offset, dest, size);
}

void SubReader::ReadString(long offset, std::string& dest)const {
    pos_.Reader().ReadString( pos_.Offset()+offset, dest);
}

SimpleBinaryPosition SubReader::Begin() const {
    return SimpleBinaryPosition(*this,0);
}

SimpleBinaryPosition SubReader::Pos(long offset) const {
    return SimpleBinaryPosition(*this,offset);
}

