#include "binaryData.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include "dataVector.h"


Data::Data (const BinaryReader &p, long size):data(size) {
    Writer().Write(p,size);
}

Data::Data(long size):data(size) {
}

void Data::Resize(long size) {
    this->data.Resize(size);
}

string Data::HexCode() { 
    ostringstream hexstring;
    for ( long i=0; i< data.Size(); ++i) { 
        hexstring << uppercase << hex << setw(2) << setfill('0') 
                  << int(data[i]);
    }
    return hexstring.str();
}

