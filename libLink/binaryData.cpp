#include "binaryData.h"
#include <sstream>
#include <iomanip>


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
        hexstring << hex << setw(2) << setfill('0') 
                  << int(data[i]);
    }
    return hexstring.str();
}
