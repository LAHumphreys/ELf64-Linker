#include "dataReader.h"

DataReader::DataReader(void *data, long l)
{
    len = l;
    rawData = reinterpret_cast<const unsigned char *>(data);
}

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
