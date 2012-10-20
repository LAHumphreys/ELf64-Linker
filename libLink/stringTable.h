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
        
        StringTable(string name);
        ~StringTable();
        string Name() { return name; }
        long AddString(const char * str);
        void WriteTable(BinaryWriter& writer);
        void WriteTable(BinaryWriter&& writer) {
            WriteTable(writer);
        };
        long Size() { return writePtr;}
    private:
        std::vector<ElfString *> strings;
        long writePtr;
        string name;
};
