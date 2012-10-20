#include "binaryData.h"
#include <sstream>
#include <iomanip>
#include <cstring>


Data::Data (const BinaryReader &p, long size) {
    data.resize(size);
    Writer().Write(p,size);
}

string Data::HexCode() { 
    ostringstream hexstring;
    for ( long i=0; i< data.Size(); ++i) { 
        hexstring << hex << setw(2) << setfill('0') 
                  << int(data[i]);
    }
    return hexstring.str();
}

