#include <cstring>

#ifndef STR_TAB_H
   #include "stringTable.h"
#endif

StringTable::StringTable ( ) {
    writePtr=0;
    strings.reserve(1000);
}
StringTable::~StringTable ( ) {
    // Clear out the heap - we know all these are allocated since we
    // created them
    for ( auto str: strings ) {
        delete [] str->str;
        delete str;
    }
}

int StringTable::AddString(const char * str) {
    StringTable::ElfString *string = new StringTable::ElfString;

    // Build the ElfString
    string->size = strlen(str) + 1; // we need to write the null char
    string->offset = this->writePtr;
    string->str = new char[string->size];
    
    // set the string value
    memcpy(string->str,str,string->size);

    strings.insert(strings.end(),string);
    this->writePtr += string->size;

    return string->offset;
}
