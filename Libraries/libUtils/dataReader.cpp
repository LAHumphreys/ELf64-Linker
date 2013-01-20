#include "dataReader.h"

DataReader::DataReader(void *data, long l)
{
    len = l;
    rawData = reinterpret_cast<const unsigned char *>(data);
}

//
//void DataReader::Read(long offset, void*dest, long size)  const
//{
//    if ( offset + size >= this->length ) {
//        throw "Invalid read: source is not long enough";
//    } else if ( offset < 0)  {
//        throw "Invalid read from object: -'ve offset";
//    }else {
//        memcpy(dest,rawData+offset,size);
//    }
//}
//
//void DataReader::ReadString(long offset, std::string&dest)  const
//{
//    if ( offset >= this->length ) {
//        throw "Invalid read: source is not long enough";
//    } else if ( offset < 0)  {
//        throw "Invalid read from object: -'ve offset";
//    }else {
//        dest = reinterpret_cast<const char *>(rawData+offset);             
//    }
//}
//
//unsigned char DataReader::Get(long offset)  const
//{
//    if ( offset >= this->length ) {
//        throw "Invalid read: source is not long enough";
//    } else if ( offset < 0)  {
//        throw "Invalid read from object: -'ve offset";
//    }else {
//        return  rawData[offset];
//    }
//}
//
long DataReader::Next ( long offset, unsigned char c) const {
    long i;
    for (i = offset; i < len; ++i) {
        if ( rawData[i] == c ) 
            return i;
    }
    return i;
}

long DataReader::Last ( long offset, unsigned char c ) const {
    long i;
    for (i = offset; i >= 0; --i) {
        if ( rawData[i] == c ) 
            return i;
    }
    return i;
}
