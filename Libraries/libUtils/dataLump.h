#ifndef __DATA_LUMP_H
#define __DATA_LUMP_H
#include "dataReader.h"
#include "dataWriter.h"
#include "fileLikeObject.h"


template<int size>
class DataLump: public virtual FileLikeObject, public DataWriter, public DataReader
                                   
{
public:
     DataLump(): DataWriter(rawData), 
                 DataReader(rawData,size)
    {
        this->Fill(0,'\0',size);
    }
    const unsigned char* RawData() const;
    unsigned char* RawData();
private:
     unsigned char rawData[size];
};

class DataIO: public virtual FileLikeObject, public DataWriter, public DataReader
                                   
{
public:
     DataIO(unsigned char* rawData, long size)
         : DataWriter(rawData), DataReader(rawData,size){}
};
#endif
