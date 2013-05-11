#include "dataVector.h"
#include <vector>
#include <cstring>

using namespace std;


DataVector::DataVector(long size)
   : vector<unsigned char>(size)
{
    for ( int i=0; i< size; i++) {
        this->operator[](i) = 'l';
    }
}
void DataVector::Write(long offset, const void *src, long size){
    if (offset + size >= this->size())
        this->resize(offset+size);
    for (long i=0; i<size; i++) {
        (*this)[i+offset] = 
                   reinterpret_cast<const unsigned char *>(src)[i];
    }
}

void DataVector::Put(long offset, unsigned char c) {
    if (offset >= this->size())
        this->resize(offset+1);
    (*this)[offset] = c;
}

void DataVector::Fill(long offset, unsigned char c, long count){
    if (offset + count >= this->size())
        this->resize(offset + count);
    for (long i=offset; i<(offset + count); i++) {
        (*this)[i] = c;
    }
}

void DataVector::Read(long offset, void *dest, long size) const {
    memcpy(dest,this->data()+offset,size);
}

unsigned char DataVector::Get(long offset) const {
    return (*this)[offset];
}

void DataVector::ReadString(long offset, std::string& dest)const {
    dest = reinterpret_cast<const char *>(this->data()+offset);
}

long DataVector::Size()const {
    return this->size();
}

long DataVector::Next( long offset, unsigned char c) const {
    long i;
    for (i = offset; i < this->size(); ++i) {
        if ( (*this)[i] == c ) 
            return i;
    }
    return i;
}

long DataVector::Last( long offset, unsigned char c) const {
    long i;
    for (i = offset; i >= 0; --i) {
        if ( (*this)[i] == c ) 
            return i;
    }
    return i;
}

void DataVector::Resize(long size)
{
    long fillStart=0;
    long toFill =0;
    if ( size > this->Size() )
    {
        toFill = size - this->Size();
        fillStart = this->Size();
    }
    this->resize(size);
    this->Fill(fillStart,'\0',toFill);
}
