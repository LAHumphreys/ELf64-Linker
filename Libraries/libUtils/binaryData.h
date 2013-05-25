#ifndef DATA_H
#define DATA_H

#include <sstream>
#include "binaryReader.h"
#include "binaryWriter.h"
#include "dataVector.h"
using namespace std;

class Data {
friend class DataWriter;
public:
    Data (const BinaryReader& p, long size);
    Data (long size);

    void Resize(long size);

    BinaryWriter Writer(){return data.Writer();}
    BinaryReader Reader(){return data.Reader();}
    const unsigned char * Raw() { return data.RawData(); }
    long Size() { return data.Size(); }
    string HexCode();

private:
    DataVector data; 
};

#endif
