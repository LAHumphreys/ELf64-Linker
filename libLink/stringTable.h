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
    private:
        std::vector<ElfString *> strings;
        long writePtr;
};
