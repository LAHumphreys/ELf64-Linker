#ifndef X86_64Parser
    #include "x86Parser.h"
#endif
#include <string>
#include <fstream>
using namespace std;
X86Parser::~X86Parser() {
    delete header;
}
X86Parser::X86Parser (string fname ) : reader(fname) {
    if ( reader.GetNextLine() != "LINK" ) {
        throw "Invalid Link file";
    } 
    // read the header line
    header = new ElfHeaderX86_64(reader.GetNextLine());
    /*if ( header->nprog == 0 ) {
        throw "Invlaid LINK header line in LINK file";
    }*/
}

void X86Parser::Write(string fname) {
    ofstream file(fname, ios::out | ios::binary);
    header->Write(file);
}
