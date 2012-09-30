#ifndef STR_TAB_H
   #define STR_TAB_H
#endif
#include <vector>

class StringTable {
    public:
        struct ElfString {
        public: 
            char * str;
            int size;
            int offset;
        };
        /**
            \fn      AddString
            \brief   Add a string to the string table
            @param[in]   str     A pointer to a constant string
            @return The address (offset within the string table) the string was inserted at
        */
        
        StringTable();
        ~StringTable();
        int AddString(const char * str);
    private:
        std::vector<ElfString *> strings;
        int writePtr;
};
