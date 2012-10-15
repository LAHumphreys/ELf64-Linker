#ifndef DATA_H
#define DATA_H

#include "symbol.h"
#include <sstream>
#include "binaryReader.h"
using namespace std;

class Data {
    public:
        Data (const BinaryReader& p, long size);
        ~Data();

        const unsigned char * Raw() { return data; }
        string HexCode();
    private:
        unsigned char * data; 
        long size;
};

#endif
