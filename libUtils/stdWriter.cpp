#include "stdWriter.h"
StdWriter::StdWriter(ofstream &f): file(f) {
}

void StdWriter::Put(long offset, unsigned char c) {
    this->file.seekp(offset, ios_base::beg);
    this->file.put(c);
}

void StdWriter::Fill(long offset, unsigned char c, long count ) {
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
