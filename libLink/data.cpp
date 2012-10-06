#include "data.h"
#include <sstream>

Data::Data (ElfReader &reader, long offset, long size) {
    data = new unsigned char[size];
    reader.Read(offset,data,size);
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
