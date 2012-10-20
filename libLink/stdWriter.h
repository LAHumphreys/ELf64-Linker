#ifndef STD_WRITER_H
#define STD_WRITER_H
#include <fstream>
#include "binaryWriter.h"

using namespace std;

class StdWriter: public FileLikeWriter {
public:
    StdWriter (ofstream &f);
    virtual void Write(long offset, const void *src, long size);
    virtual void Flush();
    virtual void Put(long offset, unsigned char c);
    virtual void Fill(long offset, unsigned char c, long count);

private:
    ofstream &file;
};

#endif
