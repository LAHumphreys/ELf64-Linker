#include <string>
#include <regex>
#include <fstream>

using namespace std;

#ifndef LINK_READER
    const char *const  NO_MORE_DATA = "__EOF__"; // returned by read fns
    const char *const  BlankData = "__BLANK__"; // marks an empty slot
    #define LINK_READER
#endif


/**
    \class    LinkReader
    \brief    An object to read data, linewise, from a LINK fomatted file
    \details  Loads a link file into memory, and provides member functions to access members.
*/
class LinkReader {
public:
    void test();
    /**
        \fn      LinkReader
        \brief   LinkReader constructor
        \details Read in the file, fname , and convert the memebers into useful
                 data structures (and doing the conversion to binary were necessary)
    
        @param[in]   fname     Path to a LINK file to open
        @return n/a
    */
    
    LinkReader (string fname);


    /**
        \fn      GetNextLine
        \brief   Get the next valid line from the input file
        @return std::string (the next valid line)
    */
    string GetNextLine();
    virtual ~LinkReader (){}


protected:
    /**
        \fn      ReadLine
        \brief   Read a line from the file, do NO validation
        @return  string
    */
    string ReadLine();
private:
    // -- Data --
    ifstream f;
    // Yeahh I know - this is overkill, but I couldn't help
    // myself: initialised in constructor
    regex commentLine;
    regex emptyLine;

};
