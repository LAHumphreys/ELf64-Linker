using namespace std;
#include <cstring>
#include <iostream>
#include "tester.h"
#include <sstream>
#include "stdWriter.h"
#include <cstdio>

//Tests
template<class Writer>
int VerifyFill(testLogger& log);

//Validators
bool ValidateMatch( const char *str1, const char *str2, 
                                      long len,
                                      testLogger& log);
bool ValidateWrite(OFStreamWriter& writer, const char *compString, 
                                           long len,
                                           testLogger& log);


int main(int argc, const char *argv[])
{
    Test("Basic fills",  (loggedTest)VerifyFill<OFStreamWriter>).RunTest();
    return 0;
}

// Define a validator for each class to be tested
// The object need not be in a usable state after validation
bool ValidateWrite(OFStreamWriter& writer, const char *compString, 
                                           long len,
                                           testLogger& log) 
{
     writer.close();
	 ifstream infile(writer.Fname());
	 char *dataString = new char[len];
     infile.read(dataString,len);
     bool match =  ValidateMatch(dataString, compString, len, log);
     delete [] dataString;
     return match;
}

bool ValidateMatch( const char *str1, const char *str2, 
                                      long len,
                                      testLogger& log)  
{
    bool match = true;
    long i;
    for (i = 0; i < len; i++) {
        log << i << ": " << str1[i] << " , "  << str2[i] << endl;
        if ( str1[i] != str2[i] ) {
            log << "Character missmatch!" << endl;
            match = false;
            break;
        }
    }
    if ( match ) {
         log << "arrays matched " << endl;
    }
    return match;
}


// Define a generator for each class to be tested
OFStreamWriter* Generator() {
    static long count = 0;
    count++;
    stringstream fname;
    fname << "test" << count << ".tmp";
    fname.flush();
    return new OFStreamWriter(fname.str().c_str());
}


// Define a Delete for each class
void Delete ( OFStreamWriter* writer ) {
    remove(writer->Fname().c_str());
    delete writer;
}


template<class Writer>
int VerifyFill(testLogger& log) {
    long ret = 0;
	Writer *writer = Generator();
	log << "Writing a block, starting at 0" << endl;
	writer->Fill(0,'>',100);
    char * refstring = new char[100];
    memset(refstring,'>',100);
	writer->Flush();
    if ( !ValidateWrite(*writer,refstring,100,log) ) {
        log << "Failed to fill from start" << endl;
        ret =  1;
    }
    Delete(writer);

    if ( ret == 0 ) {
       writer = Generator();
       writer->Fill(50,'>',50);
       memset(refstring,'\0',50);
       writer->Flush();
       if ( !ValidateWrite(*writer,refstring,100,log) ) {
           log << "Failed to fill from start" << endl;
           ret =  1;
       }
       Delete(writer);
    }
    delete [] refstring;
    return ret;
}

template<class Writer>
long VerifyPut(testLogger& log) {
	// Verify forward write
	Writer("test.tmp");
	const char * const testStr = "Hello World!\0Another String";
	const char * part1 = testStr;
	const char * part2 = testStr + 1 + strlen(part1);
	cout << part1 << endl;
	cout << part2 << endl;
}
