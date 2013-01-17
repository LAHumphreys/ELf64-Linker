#include <cstring>
#include "binaryWriter.h"

#ifndef STR_TAB_H
   #include "stringTable.h"
#endif

StringTable::StringTable () {
    writePtr=1;//first byte is defined to hold the null char
    strings.reserve(1000);
}
StringTable::~StringTable ( ) {
    // Clear out the heap - we know all these are allocated since
    // we created them
    for ( auto str: strings ) {
        delete [] str->str;
        delete str;
    }
}

long StringTable::AddString(const char * str) {
    StringTable::ElfString *string = new StringTable::ElfString;

    // Build the ElfString
      
    string->size = strlen(str) + 1; // +1 for null char
    string->offset = this->writePtr;
    string->str = new char[string->size];
    
    // set the string value
    memcpy(string->str,str,string->size);

    strings.insert(strings.end(),string);
    this->writePtr += string->size;

    return string->offset;
}

void StringTable::WriteTable(BinaryWriter& writer) {
    writer.Write("\0",1);
    for (ElfString* & str : this->strings) {
        writer.Write(str->str,str->size);
        writer += str->size;
    }
}
