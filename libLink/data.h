#ifndef DATA_H
#define DATA_H

#include "symbol.h"
#include <sstream>
#include "elfReader.h"
using namespace std;

class Data {
    public:
        Data (ElfReader &reader, long offset, long stable);
        ~Data();

        const unsigned char * Raw() { return data; }
        string HexCode();
    private:
        unsigned char * data; 
        long size;
};

#endif
