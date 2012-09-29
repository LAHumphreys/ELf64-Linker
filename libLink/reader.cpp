#include <string>
#include <iostream>
#include <fstream>
#ifndef LINK_READER
   #include "reader.h"
#endif

using namespace std;

void LinkReader::test () {
    string l = GetNextLine();
    cout << l << endl;
}
LinkReader::LinkReader(string fname): 
     commentLine(" *#.*"), emptyLine(" *")
{
    f.open(fname);
}

string LinkReader::GetNextLine () {
    string line;
    do
    {
        line = ReadLine();
    } while ((not f.eof()) and (regex_match(line,commentLine) or 
                                regex_match(line,emptyLine)    ) );
    return line;
}

string LinkReader::ReadLine () {
    string line = "";
    size_t bufSize = 1024;
    char *buf = new char[bufSize];
    if ( f.eof() ) {
        // If we've got to the end of the file just hand back blank data.
        return NO_MORE_DATA;
    } {
        do {
           f.getline(buf, bufSize);
           line += buf;
           // fail is set if the buf limit is hit
        } while (not f.eof() and f.good() and f.fail());
        return line;
    }
}
