#include "reader.h"
#include "elfHeader.h"
#ifndef X86_64Parser
    #define X86_64Parser
#endif
class X86Parser {
public:
    X86Parser (string fname);
    virtual ~X86Parser();
    void Write(string fname);


private:
   LinkReader reader;
   ElfHeaderX86_64 *header;
    /* data */
};
