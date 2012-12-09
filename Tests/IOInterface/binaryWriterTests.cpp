#include "tester.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include "dataVector.h"
#include "binaryWriter.h"
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
int VerifyWrites();
int VerifyStringWrites();
int VerifyBoundraryFinder();

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
    Test("Verify Writes", VerifyWrites).RunTest();
    Test("Verify String Writes", VerifyStringWrites).RunTest();
    Test("Verify Alignment", VerifyBoundraryFinder).RunTest();
    return 0;
}

long VerifyOffset(BinaryWriter& writer, long offset, 
                                              string msg,
                                              char c) 
{
    long statedOffset = writer.Offset();
    long castedOffset = (long)writer;
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
        err << "\n(long)writer returned an unexpected value: ";
        err << castedOffset;
        err << " ( I expected: ";
        err << offset;
        err << " )";
        throw TestError(err.str(),castedOffset);
    }
    if ( offset < DATA_SIZE  ) {
        char old = data[offset];
        writer.Write("c",1);
        char got = data[offset];
        if ( got != 'c' ) {
            ostringstream err;
            err << msg;
            err << "\n[] returned an unexpected character: ";
            err << got;
            err << " ( I expected: ";
            err << 'c';
            err << " )";
            throw TestError(err.str(),castedOffset);
        }
        writer.Write(&old,1);
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

long VerifyOffset(BinaryWriter& writer, long offset, string msg) {
    return VerifyOffset(writer,offset,msg,char(offset));
}

int VerifyPositionFinders() {
    try {
        BinaryWriter pos(data);
        BinaryWriter other = pos.operator+(DATA_SIZE);
        VerifyOffset(pos,0, "Initialisation");
        BinaryWriter b = other.Begin();
        VerifyOffset(b,0,"Begin()");
        for (int i = 0; i < DATA_SIZE; i++) {
            BinaryWriter newReader = other.Pos(i);
            pos = i;
            VerifyOffset(newReader,i,"= Pos(i)");
            VerifyOffset(pos,i, " = i");
        }

    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}

int VerifyBoundraryFinder() {
    try {
        BinaryWriter pos(data,50);
        if (   pos.NextBoundrary(10) != pos
            || pos.NextBoundrary(2) != pos
            || pos.NextBoundrary(1) != pos
            || pos.NextBoundrary(50) != pos ) 
        {
            cout << "moved off a valid boundrary!" << endl;
            return 1;
        }
        if ( pos.NextBoundrary(3) != pos.operator+(1) ) {
            cout << "Failed to move to the correct boundrary";
            cout << " ( " << pos.NextBoundrary(3).Offset() << endl;
            return 2;
        }
        BinaryWriter pos0(data,0);
        if (   pos0.NextBoundrary(10) != pos0
            || pos0.NextBoundrary(2) != pos0
            || pos0.NextBoundrary(1) != pos0
            || pos0.NextBoundrary(50) != pos0 ) 
        {
            cout << "moved off a valid boundrary!" << endl;
            return 3;
        }
        BinaryWriter pos2(data,2);
        if ( pos2.NextBoundrary(4) != pos2.Pos(4)) {
            cout << "Failed to move to the correct boundrary";
            cout << " ( " << pos2.NextBoundrary(3).Offset() << endl;
            return 4;
        }
        BinaryWriter pos12(data,12);
        if ( pos12.NextBoundrary(8) != pos12.Pos(16)) {
            cout << "Failed to move to the correct boundrary";
            cout << " ( " << pos12.NextBoundrary(8).Offset() << endl;
            return 5;
        }
    } catch (TestError& e) {
        return e.Error();
    }
}

int VerifyAssignments() {
    try {
        BinaryWriter pos(data);
        BinaryWriter other = pos.Pos(5);
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
        BinaryWriter pos(data);
        pos+=1;
        BinaryWriter other = pos.Pos(5);
        BinaryWriter p0 = pos.operator+(5);
        VerifyOffset(p0,6,"+ int");
        VerifyOffset(pos,1,"+ int (no change)");
        BinaryWriter p = pos + other;
        VerifyOffset(p,6,"+ other");
        VerifyOffset(pos,1,"+ other (no change)");
        BinaryWriter p2 = other.operator-(3);
        VerifyOffset(p2,2,"- int");
        VerifyOffset(other,5,"- int (no change)");
        BinaryWriter p3 = other - pos;
        VerifyOffset(p3,4,"- other");
        VerifyOffset(other,5,"- int (no change)");
    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}

int VerifyWrites() {
    try {
        DataVector out(100);
        DataVector zeroes(100);
        DataVector twos(100);
        DataVector cs(100);
        FillContainer(zeroes,100,0);
        FillContainer(twos,100,2);
        FillContainer(cs,100,'c');

        // Fill
        for (int i = 0; i < 100; i++) {
            out[i] = data[i];
        }
        BinaryWriter beg(out);
        BinaryWriter off = beg.operator+(50);
        BinaryWriter off2 = beg.operator+(75);
        beg.Fill(20);
        off.Fill(20,2);
        off2.FillTo(off.Begin() + out.Reader().Find(95),'c');
        
        cout << "front" <<endl;
        VerifyData((char *)out.data(),(char *)zeroes.data(),20);
        cout << "data" << endl;
        VerifyData((char *)out.data()+20,(char *)data.data()+20,30);
        cout << "twos" << endl;
        VerifyData((char *)out.data()+50,(char *)twos.data(),20);
        cout << "padding" <<endl;
        VerifyData((char *)out.data()+70,(char *)data.data()+70,5);
        cout << "FillTo" <<endl;
        VerifyData((char *)out.data()+75,(char *)cs.data()+75,20);
        cout << "back" <<endl;
        VerifyData((char *)out.data()+95,(char *)data.data()+95,5);


        // Write
        for (int i = 0; i < 100; i++) {
            out[i] = 0;
        }
        BinaryWriter wbeg(out);
        BinaryWriter woff = beg.operator+(50);
        BinaryWriter woff2 = beg.operator+(75);


        wbeg.Write(data.data(),100);
        woff.Write(zeroes.data(),20);
        woff2.Write(cs.Reader().Begin(),20);

        cout << "write" <<endl;
        VerifyData((char *)out.data(),(char *)data.data(),50);
        cout << "offset write" << endl;
        VerifyData((char *)out.data()+50,(char *)zeroes.data(),20);
        cout << "Write reader" << endl;
        VerifyData((char *)out.data()+75,(char *)cs.data(),20);
        cout << "back" << endl;
        VerifyData((char *)out.data()+95,(char *)data.data()+95,5);

    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}

int VerifyStringWrites() {
    try {
        char str[] = "Hello World!\0++IGNORED++";
        //don't forget the null char
        int len = strlen(str) + 1;
        string stds = "some other string";
        DataVector out(100);
        DataVector out2(100);
        for (int i = 0; i < 100; i++) {
            out[i] = data[i];
            out2[i] = data[i];
        }

        BinaryWriter w(out);
        BinaryWriter pos = w.operator+(50);
        BinaryWriter w2(out2);
        w2+=10;

        w.WriteString(str);
        pos.WriteString(stds);
        w2.WriteString(out.Reader().Pos(50));

        cout << "array write" << endl;
        VerifyData((char *)out.data(),str,len);
        cout << "left alone" << endl;
        VerifyData( (char *)out.data()+len, (char *)data.data() + len,
                                            50-len);
        cout << "string write" << endl;
        VerifyData((char *)out.data()+50,stds.c_str(),stds.length()+1);
        cout << "back" << endl;
        VerifyData( (char *)out.data() + 50 + stds.length() + 1,
                    (char* )data.data()+ 50 + stds.length() + 1,
                    50 -1 - stds.length());
        cout << "skipped" << endl;
        VerifyData((char *)out2.data(),(char *)data.data(),10);
        cout << "reader write" << endl;
        VerifyData((char *)out2.data()+10,stds.c_str(),stds.length());
        cout << "back (2) " << endl;
        VerifyData( (char *)out2.data() + 10 + stds.length() + 1,
                    (char *)data.data() + 10 + stds.length() + 1,
                    100 - 10 - stds.length() -1 );

    } catch (TestError& e) {
        return e.Error();
    }
    return 0;
}
