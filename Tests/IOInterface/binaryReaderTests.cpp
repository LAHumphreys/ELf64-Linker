#include "tester.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include "dataVector.h"
#include "binaryReader.h"
#include <sstream>
#include <string>
#include <cstring>

/*
 * Tests should be run after the datavector object has been verified by the
 * test suite
 *
 */ 

const int DATA_SIZE=128;
DataVector data(DATA_SIZE);
char * dataptr = (char *)data.data();
class TestError {
public:
    TestError (string s, long i) {
        this->err = s;
        this->errCode = i;
    };
    int Error() { 
        cout << err << endl;
        return errCode==0? 255: errCode;
    }

private:
    string err;
    long errCode;
};

int VerifyPositionFinders();
int VerifyAssignments();
int VerifyArithmetic();
int VerifySearch();
int VerifyReads();

using namespace std;
int main(int argc, const char *argv[])
{
    // Set up the data vectory
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = char(i);
    }
    Test("Return positions in file", VerifyPositionFinders).RunTest();
    Test("Verify Assignment Operators", VerifyAssignments).RunTest();
    Test("Verify Arithmetic Operators", VerifyArithmetic).RunTest();
    Test("Verify Searches", VerifySearch).RunTest();
    Test("Verify Reads", VerifyReads).RunTest();
    return 0;
}

long VerifyOffset(const BinaryReader& reader, long offset, 
                                              string msg,
                                              char c) 
{
    long statedOffset = reader.Offset();
    long castedOffset = (long)reader;
    if ( statedOffset != offset ) {
        ostringstream err;
        err << msg;
        err << "\nreader.Offset returned an unexpected value: ";
        err << statedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),statedOffset);
    }
    if ( castedOffset != offset ) {
        ostringstream err;
        err << msg;
        err << "\n(long)reader returned an unexpected value: ";
        err << castedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),castedOffset);
    }
    if ( offset < reader.End() ) {
        char got = reader.Get();
        if ( got != c ) {
            ostringstream err;
            err << msg;
            err << "\nGet returned an unexpected character: ";
            err << got;
            err << " ( I expected: ";
            err << c;
            err << " )";
            throw TestError(err.str(),castedOffset);
        }
    }
    return 0;
}

long VerifyData(const char* d1, const char *d2, long size) {
    for (int i = 0; i < size; i++) {
        if ( d1[i] != d2[i] ) {
            ostringstream err;
            err << "Missmatch in data streams at position ";
            err << i << ": " << d1[i];
            err << " ( " << int(d1[i]) << " ) ";
            err << " and " << d2[i];
            err << " ( " << int(d2[i]) << " ) " << endl;
            throw TestError(err.str(),i);
        }
    }
}

long VerifyOffset(const BinaryReader& reader, long offset, 
                                              string msg)
{
    return VerifyOffset(reader,offset,msg,char(offset));
}

int VerifyPositionFinders() {
    try {
        BinaryReader pos(data);
        BinaryReader other = pos.End();
        VerifyOffset(pos,0, "Initialisation");
        VerifyOffset(other,DATA_SIZE, "End");
        VerifyOffset(other.Begin(),0,"Begin()");
        for (int i = 0; i < DATA_SIZE; i++) {
            BinaryReader newReader = other.Pos(i);
            pos = i;
            VerifyOffset(newReader,i,"= Pos(i)");
            VerifyOffset(pos,i, " = i");
        }

    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}

int VerifyAssignments() {
    try {
        BinaryReader pos(data);
        BinaryReader other = pos.Pos(5);
        pos+=5;
        VerifyOffset(pos,5,"+= int");
        pos+= other;
        VerifyOffset(pos,10,"+= other");
        pos-= other;
        VerifyOffset(pos,5,"-= other");
        pos-= 1;
        VerifyOffset(pos,4,"-= int");
        pos = 123;
        VerifyOffset(pos,123,"= int");
    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}


int VerifyArithmetic() {
    try {
        BinaryReader pos(data);
        pos+=1;
        BinaryReader other = pos.Pos(5);
        VerifyOffset(pos.operator+(5),6,"+ int");
        VerifyOffset(pos,1,"+ int (no change)");
        VerifyOffset(pos+other,6,"+ other");
        VerifyOffset(pos,1,"+ other (no change)");
        VerifyOffset(other.operator-(3),2,"- int");
        VerifyOffset(other,5,"- int (no change)");
        VerifyOffset(other-pos,4,"- other");
        VerifyOffset(other,5,"- int (no change)");
    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}

int VerifySearch() {
    try {
        BinaryReader r(data);
        VerifyOffset(r.Find(25),25,"find");
        VerifyOffset(r.Pos(26).Find(25),r.End(),"off find");
        VerifyOffset(r.Pos(26).RFind(25),25, "off rfind");
        VerifyOffset(r.RFind(25),r.Begin(), "rfind");
    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}
int VerifyReads() {
    try {
        BinaryReader r(data);
        cout << "Dup tests" << endl;
        char * d = (char *)r.Dup(10);
        VerifyData(d, dataptr,10);
        delete [] d;
        r+= 10;
        d = (char *)r.Dup(10);
        VerifyData(d, dataptr + 10,10);
        delete [] d;

        DataVector dv(100);
        dv.Fill(0,'*',100);
        dv.Put(50,'\0');
        dv.Put(20,'\0');
        BinaryReader rd(dv);

        cout << " String tests" << endl;
        string s = rd.Begin().ReadString();
        VerifyData(s.c_str(),(char *)dv.data(),21);
        if ( s.length() != 20)
            throw(TestError("String has invalid length",s.length()));
        cout << "Read by ref" << endl;
        rd.Pos(21).ReadString(s);
        VerifyData(s.c_str(),(char *)dv.data()+21,30);
        if ( s.length() != 29)
            throw(TestError("String has invalid length", s.length()));
        cout << "Append to string" << endl;
        rd.Begin().AppendString(s);
        cout << "Front" << endl;
        VerifyData(s.c_str(),(char *)dv.data()+21,29);
        cout << "Back" << endl;
        VerifyData(s.c_str(),(char *)dv.data(),20);
        if ( s.length() != 49)
            throw(TestError("String has invalid length", s.length()));

        cout << "Checking null string" << endl;
        rd.Pos(20).ReadString(s);
        if ( s.length() != 0)
            throw(TestError("String has invalid length", s.length()));

        cout << "Read to array" << endl;
        char * buf = new char[100];
        memset(buf,'\0',100);
        r.Begin().Read(buf,100);
        VerifyData(buf,dataptr, 100);
        memset(buf,'\0',100);
        r.Pos(24).Read(buf,25);
        VerifyData(buf,dataptr+24, 25);
        delete [] buf;

        cout << "Read to writer" << endl;
        DataVector dataout(128);
        BinaryWriter w = dataout.Writer().Begin();
        r.Begin().Read(w,50);
        w+=10;
        r.Pos(25).Read(w,50);
        VerifyData((char *)dataout.data()+10,dataptr+25, 50);


    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}

