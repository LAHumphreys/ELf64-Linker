#include "binaryData.h"
#include <sstream>

Data::Data (const BinaryPosition &p, long size) {
    data = new unsigned char[size];
    p.Read(data,size);
    this->size = size;
}

Data::~Data() {
    if (data) delete [] data;
}

string Data::HexCode() { 
    ostringstream hexstring;
    for ( long i=0; i< size; ++i) { 
        hexstring << hex << int(data[i]);
    }
    return hexstring.str();
}
