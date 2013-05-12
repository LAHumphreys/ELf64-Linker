#include "tester.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include "dataVector.h"
#include "binaryReader.h"
#include <sstream>
#include <string>
#include <cstring>
#include "defer.h"
#include "dataWriter.h"

/*
 * Tests should be run after the datavector object has been verified by the
 * test suite
 *
 */ 

const int DATA_SIZE=128;
DataVector data(DATA_SIZE);
char * dataptr = (char *)data.RawData();
class TestError {
public:
    TestError (string s, long i) {
        this->err = s;
        this->errCode = i;
    };
    int Error( testLogger& log) { 
        log << err << endl;
        return errCode==0? 255: errCode;
    }

private:
    string err;
    long errCode;
};

int VerifyPositionFinders( testLogger& log);
int VerifyAssignments( testLogger& log);
int VerifyArithmetic( testLogger& log);
int VerifySearch( testLogger& log);
int VerifyReads( testLogger& log);
int VerifyStringPull( testLogger& log);
int VerifyPODPull( testLogger& log);
int VerifyReadLine (testLogger& log);
int  VerifyReadLineForWriter(testLogger& log);

using namespace std;
int main(int argc, const char *argv[])
{
    // Set up the data vectory
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = char(i);
    }
    Test("Return positions in file",  (loggedTest)VerifyPositionFinders).RunTest();
    Test("Verify Assignment Operators",  (loggedTest)VerifyAssignments).RunTest();
    Test("Verify Arithmetic Operators",  (loggedTest)VerifyArithmetic).RunTest();
    Test("Verify Searches",  (loggedTest)VerifySearch).RunTest();
    Test("Verify Reads",  (loggedTest)VerifyReads).RunTest();
    Test("Verify String Pulls",  (loggedTest)VerifyStringPull).RunTest();
    Test("Verify POD Pulls",  (loggedTest)VerifyPODPull).RunTest();
    Test("Verify ReadLine",  (loggedTest)VerifyReadLine).RunTest();
    Test("Verify ReadLine using writer",  (loggedTest)VerifyReadLineForWriter).RunTest();
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

            char *str1 = new char[i+1];
            char *str2 = new char[i+1];
           char *ptr = new char[i+1];
            for ( int j=0; j<=i; j++ ) {
                if ( d1[j] == '\0' ) { 
                    str1[j] = '`';
                } else {
                    str1[j] = d1[j];
                }

                if ( d2[j] == '\0' ) { 
                    str2[j] = '`';
                } else {
                    str2[j] = d2[j];
                }
                ptr[j] = ' ';
            }

            if ( d1[i] == '\0' ) { 
                str1[i] = '`';
            } else {
                str1[i] = d1[i];
            }

            if ( d2[i] == '\0' ) { 
                str2[i] = '`';
            } else {
                str2[i] = d2[i];
            }
            ptr[i] = '^';
            str1[i+1] = '\0';
            str2[i+1] = '\0';
            ptr[i+1] = '\0';
            err << endl << str1 << endl;
            err << str2 << endl;
            err << ptr << endl;
            delete [] str1;
            delete [] str2;
            delete [] ptr;

            throw TestError(err.str(),i);
        }
    }
}

long VerifyOffset(const BinaryReader& reader, long offset, 
                                              string msg)
{
    return VerifyOffset(reader,offset,msg,char(offset));
}

int VerifyPositionFinders( testLogger& log) {
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
        return e.Error(log);
    }
    return 0;
}

int VerifyAssignments( testLogger& log) {
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
        return e.Error(log);
    }
    return 0;
}


int VerifyArithmetic( testLogger& log) {
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
        return e.Error(log);
    }
    return 0;
}

int VerifySearch( testLogger& log) {
    try {
        BinaryReader r(data);
        VerifyOffset(r.Find(25),25,"find");
        VerifyOffset(r.Pos(26).Find(25),r.End(),"off find");
        VerifyOffset(r.Pos(26).RFind(25),25, "off rfind");
        VerifyOffset(r.RFind(25),r.Begin(), "rfind");
    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

int VerifyReads( testLogger& log) {
    try {
        BinaryReader r(data);
        log << "Dup tests" << endl;
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

        log << " String tests" << endl;
        string s = rd.Begin().ReadString();
        VerifyData(s.c_str(),(char *)dv.RawData(),21);
        if ( s.length() != 20)
            throw(TestError("String has invalid length",s.length()));
        log << "Read by ref" << endl;
        rd.Pos(21).ReadString(s);
        VerifyData(s.c_str(),(char *)dv.RawData()+21,30);
        if ( s.length() != 29)
            throw(TestError("String has invalid length", s.length()));
        log << "Append to string" << endl;
        rd.Begin().AppendString(s);
        log << "Front" << endl;
        VerifyData(s.c_str(),(char *)dv.RawData()+21,29);
        log << "Back" << endl;
        VerifyData(s.c_str(),(char *)dv.RawData(),20);
        if ( s.length() != 49)
            throw(TestError("String has invalid length", s.length()));

        log << "Checking null string" << endl;
        rd.Pos(20).ReadString(s);
        if ( s.length() != 0)
            throw(TestError("String has invalid length", s.length()));

        log << "Read to array" << endl;
        char * buf = new char[100];
        memset(buf,'\0',100);
        r.Begin().Read(buf,100);
        VerifyData(buf,dataptr, 100);
        memset(buf,'\0',100);
        r.Pos(24).Read(buf,25);
        VerifyData(buf,dataptr+24, 25);
        delete [] buf;

        log << "Read to writer" << endl;
        DataVector dataout(128);
        BinaryWriter w = dataout.Writer().Begin();
        r.Begin().Read(w,50);
        w+=10;
        r.Pos(25).Read(w,50);
        VerifyData((char *)dataout.RawData()+10,dataptr+25, 50);


    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

int VerifyStringPull( testLogger& log) {
    try {
        DataVector dv(100);

        dv.Fill(0,'*',100);
        dv.Put(50,'\0');
        dv.Put(20,'\0');

        BinaryReader rd(dv);
        BinaryReader rd2(dv);

        char * read = new char[120];
        char * pulld = new char [120];
        char * opd = new char [120];

        memset(read,'1',120);
        memset(pulld,'1',120);
        memset(opd,'1',120);

        log << "First pull... " << endl;
        rd.Read(read,21);
        rd.Pull(pulld,'\0');

        VerifyData(read, pulld,120);

        log << "First op" << endl;
        rd2 >> opd;
        VerifyData(read, opd,120);

        if ( rd.Offset() != 21 ) {
            log << "Pull didn't correctly advance the offset!" << endl;
            return 1;
        }

        log << "Second pull... " << endl;

        rd.Read(read+21,30);
        rd.Pull(pulld+strlen(pulld)+1,'\0');


        VerifyData(read, pulld,120);

        log << "Second op" << endl;
        rd2 >> opd + strlen(opd) + 1;
        VerifyData(read, opd,120);

        log << "Pull to the end... " << endl;
        log << "Data size: " << dv.Size() << endl;

        rd.Read(read+51,49);
        rd.Pull(pulld+51,'\0');


        VerifyData(read, pulld,120);

        log << "Final op" << endl;
        rd2 >> opd + 51;
        VerifyData(read, opd,120);


        delete [] read;
        delete [] pulld;
        delete [] opd;
    } catch (TestError& e) {
        return e.Error(log);
    }

    return 0;
}

int VerifyPODPull( testLogger& log) {
    DataVector dv(100);
    
    dv.Fill(0,'*',100);
    BinaryWriter w(dv);
    
    int i = 1;
    long l = 10;
    float f = 100;
    double d = 1000;
    
    struct POD1 {
            int i;
            long l;
            float f;
            double d;
            long sum() { return i + l + f +d; }
            bool operator== (POD1& rhs) { 
                return memcpy(this,&rhs,sizeof(POD1)) != 0;
            }
    };

    struct POD2 {
            POD1 p1;
            int ints[5];
            POD1 p3;
            long sum () {
               long sum = 0;
               for (int i = 0; i < 5; i++) {
                   sum+=ints[i];
               }
               return p1.sum() + p3.sum() + sum;
            }
            bool operator== (POD2& rhs) { 
                return memcpy(this,&rhs,sizeof(POD2)) != 0;
            }
    };
    
    
    struct POD3 {
            POD1 p1s[5];
            POD2 p2s[5];
            long sum () {
                long sum = 0;
                for (int i = 0; i < 5; i++) {
                    sum+=p1s[i].sum() + p2s[i].sum();
                }
                return sum;
            }
            bool operator== (POD3& rhs) { 
                return memcpy(this,&rhs,sizeof(POD3)) != 0;
            }
    };
    
    POD1 p1a = {i,l,f,d};
    POD1 p1b = {2*i,2*l,2*f,2*d};
    POD2 p2 = {p1a,{1,2,3,4,5},p1b};
    POD3 p3 = { {p1a,p1b,p1a,p1b,p1a}, { p2,p2,p2,p2,p2} };
    
    // We have some chicken and egg here, assume the << operator 
    // doesn't work yet...
    log << "size of i:  " << sizeof(int) << endl;
    log << "size of l:  " << sizeof(long) << endl;
    log << "size of f:  " << sizeof(float) << endl;
    log << "size of d:  " << sizeof(double) << endl;
    log << "size of p1: " << sizeof(POD1) << endl;
    log << "size of p2: " << sizeof(POD2) << endl;
    log << "size of p3: " << sizeof(POD3) << endl;
    
    log << "Writing data...";
    w.Write(&i,sizeof(i));
    w+=sizeof(i);
    w.Write(&l,sizeof(l));
    w+=sizeof(l);
    w.Write(&f,sizeof(f));
    w+=sizeof(f);
    w.Write(&d,sizeof(d));
    w+=sizeof(d);
    w.Write(&p1a,sizeof(p1a));
    w+=sizeof(p1a);
    w.Write(&p1b,sizeof(p1b));
    w+=sizeof(p1b);
    w.Write(&p2,sizeof(p2));
    w+=sizeof(p2);
    w.Write(&p3,sizeof(p3));
    w+=sizeof(p3);
    log << "done" << endl;
    
    int ri = 30;
    long rl = 30000;
    float rf = 34345.45345;
    double rd = 3453453434.34534534;
    POD1 rp1a;
    POD1 rp1b;
    POD2 rp2;
    POD3 rp3;

    BinaryReader reader(dv);

    log << "Checking Plain types" << endl;
    reader >> ri;
    reader >> rl;
    reader >> rf;
    reader >> rd;

    if ( ri != i ) {
        log << "i missmatch, " << ri << endl;
        return 1;
    }

    if ( rl != l ) {
        log << "l missmatch, " << rl << endl;
        return 1;
    }

    if ( rf != f ) {
        log << "f missmatch, " << rf << endl;
        return 1;
    }

    if ( rd != d ) {
        log << "d missmatch,  " << rd << endl;
        return 1;
    }

    log << "Checking compound types..." << endl;

    reader >> rp1a;
    reader >> rp1b;
    reader >> rp2;
    reader >> rp3;

    if ( rp1a.sum() != p1a.sum() || !(rp1a == p1a) ) {
        log << "p1a missmatch, " << endl;;
        return 1;
    }

    if ( rp1b.sum() != p1b.sum() || !(rp1b == p1b) ) {
        log << "p1b missmatch, "<< endl;;
        return 1;
    }

    if ( rp2.sum() != p2.sum() || !(rp2 == p2) ) {
        log << "p2 missmatch, "<< endl;;
        return 1;
    }

    if ( rp3.sum() != p3.sum() || !(rp3 == p3) ) {
        log << "p3 missmatch, "<< endl;;
        return 1;
    }

    return 0;
}

int VerifyReadLine (testLogger& log) {
    try {
        char * outData = new char [256];
        char * nullData = new char [256];
        DEFER (
            delete [] outData; 
            delete [] nullData; 
        )

        memset(outData,'*',256);
        memset(nullData,'*',256);


        // Verify max limit
        BinaryReader r(data);
        r.ReadLine((unsigned char *)outData,25,128);
        VerifyData((char *)data.RawData(),outData,25);
        VerifyData(nullData+25,outData+25,100);

        r.ReadLine((unsigned char *)outData,256,128);
        VerifyData((char *)data.RawData(),outData,128);
        VerifyData(nullData+128,outData+128,100);

    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}

int VerifyReadLineForWriter (testLogger& log) {
    try {
        char * outData = new char [256];
        char * nullData = new char [256];
        DEFER (
            delete [] outData; 
            delete [] nullData; 
        )

        memset(outData,'*',256);
        memset(nullData,'*',256);

        DataWriter w(outData);


        // Verify max limit
        BinaryReader r(data);
        r.ReadLine(w,25,128);
        VerifyData((char *)data.RawData(),outData,25);
        VerifyData(nullData+25,outData+25,100);

        r.ReadLine(w,256,128);
        VerifyData((char *)data.RawData(),outData,128);
        VerifyData(nullData+128,outData+128,100);

    } catch (TestError& e) {
        return e.Error(log);
    }
    return 0;
}
