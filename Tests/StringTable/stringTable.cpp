#include <vector>
#include "tester.h"
#include "stringTable.h"
#include <cstring>
#include <iostream>

using namespace std;

int insert(stringstream& log);
int tabSize(stringstream& log);
int WriteTable(stringstream& log);

const vector<const char *> testStrings = vector<const char *>( 
      {"STRING_1", "Test string two", "the same string", "the same string" });

int main(int argc, const char *argv[])
{
    Test("It is possible to insert a string?", (loggedTest)insert).RunTest();
    Test("Validate the size function", (loggedTest)tabSize).RunTest();
    Test("Validate output table", (loggedTest)WriteTable).RunTest();
    return 0;
}

int insert(stringstream& log) {
    int lastLen=1;
    int lastLoc=0;
    StringTable tab;
    for( const char * const & str: testStrings) {
		log << "Adding: <" << str << "< (" << strlen(str) << ")...";
        int loc = tab.AddString(str);
		log << "done" << endl;
		log << "lastLoc: " << lastLoc << endl;
		log << "lastLen: " << lastLen << endl;
		log << "loc: "     << loc << endl;
        if ( lastLoc + lastLen != loc ) {
            log << "Expected " << lastLoc + lastLen;
            log << " but got " << loc << endl;
			log << "After inserting: " << str << endl;
            return 1;
        }
		lastLoc = loc;
		lastLen = strlen(str) +1;
    }

    return 0;
}

int tabSize (stringstream& log) {
    int lastLen=1;
    StringTable tab;
    for( const char * const & str: testStrings) {
		log << "Adding: >" << str << "< ...";
        tab.AddString(str);
		int len = strlen(str) +1; //null char
		log << "(" << len << ")" << "done"  << endl;
        if ( tab.Size() != lastLen + len ) {
            log << "Expected " << lastLen+ len;
            log << " but got( " << tab.Size() << endl;
            return 1;
        }
		lastLen += len;
    }
	return 0;
}

/*
 * We're building this
 * "\0<string 1>\0<string 2>\0<string 3>\0"
 */
int WriteTable (stringstream& log) {
    int lastLen=1;
    StringTable tab;
	log << "Writing string0" << endl;
	int idx0 = tab.AddString(testStrings[0]);
	log << "Writing string1" << endl;
	int idx1 = tab.AddString(testStrings[1]);
	log << "Writing string2" << endl;
	int idx2 = tab.AddString(testStrings[2]);
	int tableLength = strlen(testStrings[0]) 
	                + strlen(testStrings[1])
	                + strlen(testStrings[2])
	                + 1 + 3; // starting null, plus string terminations
	 char * charTable = new char[tableLength];
	 charTable[0] = '\0';
	 char * writePtr = charTable + 1;
	 for( int i =0; i<3; i++) {
		  log << "strcpy " << i << endl;
	      strcpy(writePtr,testStrings[i]);
	      writePtr += strlen(testStrings[i]) +1;
	 }

	 log << "declare data lump" << endl;
	 DataLump<100> outTable;
	 log << "copying table....";
	 tab.WriteTable(outTable.Begin());
	 log << "done" << endl;

     
	 const char * outArr = (const char *) outTable.RawPtr();
     // first check the two blocks are identical
	 for ( int i=0; i<tableLength; i++ ) {
		 log << i << ": " << charTable[i] << " , " << outArr[i] << endl;
         if (outArr[i] != charTable[i]) {
		     return i+1;
		 }
	 }
	 log << "All good, checking strings map" << endl;
	 const char * str0 = outArr + idx0;
	 const char * str1 = outArr + idx1;
	 const char * str2 = outArr + idx2;

	 log << str0 << " , " << testStrings[0] << endl;
	 if ( strcmp(str0,testStrings[0]) != 0 ) {
			 return -1;
	 }
	 log << str1 << " , " << testStrings[1] << endl;
	 if ( strcmp(str1,testStrings[1]) != 0 ) {
			 return -2;
	 }
	 log << str2 << " , " << testStrings[2] << endl;
	 if ( strcmp(str2,testStrings[2]) != 0 ) {
			 return -3;
	 }
	 log << "All looks good, test done" << endl;

	 delete [] charTable;
	return 0;
}

