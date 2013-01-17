#ifndef STR_TAB_H
   #define STR_TAB_H
#endif
#include <vector>
#include "section.h"

class StringTable {
    public:
        struct ElfString {
        public: 
            char * str;
            long size;
            long offset;
        };
        
        StringTable();
        ~StringTable();
        long AddString(const char * str);
        void WriteTable(BinaryWriter& writer);
        void WriteTable(BinaryWriter&& writer) {
            WriteTable(writer);
        };
        long Size() { return writePtr;}
    private:
        std::vector<ElfString *> strings;
        long writePtr;
};

/*
* The String table is just a list of null-terminated strings.
* The first byte, index 0, must hold the null character:
*    "\0StringOne\0StringTwo\0StringThree\0"
*
*/
