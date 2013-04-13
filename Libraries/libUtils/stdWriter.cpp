#include "stdWriter.h"
StdWriter::StdWriter(ostream &f): file(f) {
}

void StdWriter::Put(long offset, unsigned char c) {
    this->file.seekp(offset, ios_base::beg);
    this->file.put(c);
}

void StdWriter::Fill(long offset, unsigned char c, long count ) {
    this->file.seekp(offset, ios_base::beg);
    for (int i = 0; i < count; i++) {
        this->file.put(c);
    }
}

void StdWriter::Write (long offset, const void *src, long size) {
    this->file.seekp(offset, ios_base::beg);
    this->file.write((const char *)src,size);
}

void StdWriter::Flush() {
    this->file.flush();
}

OFStreamWriter::OFStreamWriter(const char *fname): 
                ofstream(fname, ios_base::binary | ios_base::out),
                StdWriter((ofstream& )*this),
				fileName(fname)
{
}
