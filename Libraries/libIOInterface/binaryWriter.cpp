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

void BinaryWriter::Write(const void *src, long size) {
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

void BinaryWriter::FillTo ( const BinaryWriter& stop,
                            unsigned char c ) {
    long diff = stop.offset - offset;
    if ( diff > 0) {
        Fill(diff , c);
    } 
}

void BinaryWriter::Fill(long size, unsigned char c ) {
    this->file.Fill(offset,c,size);
}

void BinaryWriter::WriteString(const BinaryReader &pos) {
    string buf = pos.ReadString();
    this->file.Write(this->offset, buf.c_str(),buf.length()+1);
}

void BinaryWriter::WriteString(const std::string& src) {
    this->file.Write(offset,src.c_str(), src.length()+1);
}

void BinaryWriter::WriteString(const void *src) {
    this->file.Write(offset,src,strlen((const char *)src)+1);
}

BinaryWriter BinaryWriter::NextBoundrary(long alignment) const {
    long diff = offset % alignment;
    if (diff != 0 ) {
        // Move to next boundrary
        return BinaryWriter(file,offset + (alignment-diff));
    } else {
        // already on a boundrary
        return BinaryWriter(file,offset);
    }
}

// Reposition the pointer
BinaryWriter& BinaryWriter::operator+=( long additionalOffset) 
{
    offset += additionalOffset;
    return *this;
}

BinaryWriter& BinaryWriter::operator-=( long additionalOffset) 
{
    offset -= additionalOffset;
    return *this;
}

BinaryWriter& BinaryWriter::operator=(long offset) {
    this->offset = offset;
    return *this;
}

long BinaryWriter::operator%(long alignment) const {
    return offset % alignment;
}

BinaryWriter BinaryWriter::Begin() const {
    return BinaryWriter(file,0);
}

BinaryWriter BinaryWriter::Pos(long offset) const {
    return BinaryWriter(file,offset);
}

BinaryWriter& BinaryWriter::Push(const char* src ,char delim) {
    long len = 0;
    for (bool found=false; !found; len++ ) {
        found = (src[len] == delim);
    }
    Write(src,len);
    offset+=len;
    return *this;
}


