#include "stdReader.h"

StdReader::StdReader(istream&f ): file(f) {
    file.seekg(file.end, ios_base::beg);
    length = file.tellg();
}
void StdReader::Read(long offset, void *dest, long size) const {
    file.seekg(offset, ios_base::beg);
    file.read((char *)dest,size);
}
void StdReader::ReadString(long offset, std::string& dest) const {
    file.seekg(offset,ios_base::beg);
    getline(file,dest,'\0');
}
unsigned char StdReader::Get(long offset) const {
    this->file.seekg(offset, ios_base::beg);
    return file.get();
}

long StdReader::Size() const {
    return length;
}
long StdReader::Next( long offset, unsigned char c) const {
    this->file.seekg(offset, ios_base::beg);
    for (long i=offset; file.good(); i++) {
        while (file.good() ) {
            char got = file.get();
            if (file.good() && got==c ) {
                return i;
            }
        }
    }
    file.clear();
    return Size();
}
long StdReader::Last( long offset, unsigned char c) const {
    for (long i=offset; i>=0; i--) {
            // TODO: do some kind of buffering here?
            this->file.seekg(i, ios_base::beg);
            char got = file.get();
            if ( got == c ) 
                return i;
    }
    return -1;
}

IFStreamReader::IFStreamReader(const char *fname): 
                ifstream(fname, ios_base::binary | ios_base::in),
                StdReader((ifstream& )*this),
				fileName(fname)
{
}
