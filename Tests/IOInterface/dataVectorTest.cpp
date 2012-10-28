#include "dataVector.h"
#include "tester.h"
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int validateFill();
int validateFillAndGrow();
int validatePut();
int validatePutAndGrow();
int validateWrite();
int validateWriteAndGrow();
  
int validateRead();
int validateReadString();
int validateGet();
int validateSearch();

int main(int argc, const char *argv[])
{
    Test("Filling a vector works", validateFill).RunTest();
    Test("Filling a vector grows if necessary", validateFillAndGrow).RunTest();
    Test("Putting into a vector works", validatePut).RunTest();
    Test("Putting into a vector grows if necessary", validatePutAndGrow).RunTest();
    Test("Writing into a vector works", validateWrite).RunTest();
    Test("Writing into a vector grows if necessary", validateWriteAndGrow).RunTest();
    Test("Read from a vector works correctly", validateRead).RunTest();
    Test("Reading a string  from a vector works ", validateReadString).RunTest();
    Test("Getting from a vector works", validateGet).RunTest();
    Test("Searching for a byte works correctly", validateSearch).RunTest();
    return 0;
}

int validateFill() {
    DataVector data(50);
    vector<unsigned char> v(50);
    FillContainer(v,50,'*');
    FillContainer(data,50,'-');


    data.Fill(5,'*',40);
    auto firstDiff = mismatch(v.begin(),v.end(),data.begin());
    if ( firstDiff.first != v.begin() ) {
        return 1;
    }

    auto secondDiff = mismatch(v.begin()+5,v.end(),data.begin()+5);
    if ( secondDiff.first != v.begin()+45 ) {
        return 2;
    }

    return 0;

}

int validateFillAndGrow() {
    DataVector data(50);
    vector<unsigned char> v(100);
    FillContainer(v,100,'*');
    FillContainer(data,50,'-');

    data.Fill(0,'*',90);
    auto diff = mismatch(v.begin(),v.end(),data.begin());
    if ( diff.first != v.begin()+90 ) {
        return 1;
    }
    return 0;
}

int validatePut() {
    DataVector data(50);
    vector<unsigned char> v(50);
    FillContainer(v,50,'*');
    FillContainer(data,50,'*');

    data.Put(40,'-');
    auto diff = mismatch(v.begin(),v.end(),data.begin());
    if ( diff.first != v.begin()+40 ) {
        return 1;
    }
    return 0;
}

int validatePutAndGrow() {
    DataVector data(50);
    vector<unsigned char> v(100);
    FillContainer(v,100,'*');
    FillContainer(data,50,'*');

    data.Put(49,'*');
    data.Put(50,'*');
    data.Put(51,'*');
    data.Put(52,'-');
    auto diff = mismatch(v.begin(),v.end(),data.begin());
    int idx = diff.first - v.begin();
    if ( idx != 52 ) {
        if (idx) return idx;
        else return -1;
    }
    return 0;
}

int validateWrite(){
    DataVector data(50);
    unsigned char *source = new unsigned char[100];
    FillContainer(source,100,'*');
    FillContainer(data,50,'-');

    data.Write(5,source,40);
    for ( int i=0; i< 5; ++i ) {
        if ( data[i] != '-')
            return i ==0? 255: i;
    }
    for ( int i=5; i< 45; ++i ) {
        if ( data[i] != '*')
            return i ==0? 255: i;
    }
    if ( data[45] != '-' ) 
        return 45;

    return 0;
}

int validateWriteAndGrow() {
    DataVector data(50);
    unsigned char *source = new unsigned char[100];
    FillContainer(source,100,'*');
    FillContainer(data,50,'-');

    data.Write(5,source,90);
    for ( int i=0; i< 5; ++i ) {
        if ( data[i] != '-')
            return i ==0? 255: i;
    }
    for ( int i=5; i< 95; ++i ) {
        if ( data[i] != '*')
            return i ==0? 255: i;
    }
    return 0;
}

int validateRead() {
    DataVector data(50);
    unsigned char *dest = new unsigned char[50];
    FillContainer(dest,50,'-');
    FillContainer(data,50,'*');
    FillContainer(data,5,'+');

    data.Read(5,dest+5,40);
    for ( int i=0; i< 5; ++i ) {
        if ( dest[i] != '-') 
            return i ==0? 255: i;
    }
    for ( int i=5; i< 45; ++i ) {
        if ( dest[i] != '*')
            return i ==0? 255: i;
    }
    for ( int i=45; i< 50; ++i ) {
        if ( dest[i] != '-')
            return i ==0? 255: i;
    }
    return 0;
}

int validateReadString() {
    string source="Hello world!";
    string dest="I'll be over written";
    DataVector data(50);
    FillContainer(data,50,'*');
    data.Write(10,source.c_str(),source.length()+1);
    data.ReadString(10, dest);
    if ( dest != source ) {
        cout << dest << endl;
        cout << source << endl;
        return 1;
    }
    else
        return 0;
}

int validateGet() {
    DataVector data(50);
    FillContainer(data,50,'*');
    data[10] = '-';
    if (data.Get(10) != '-' )
        return 10;
    else
        return 0;
}

int validateSearch() {
    DataVector data(50);
    FillContainer(data,50,'*');
    data[10] = '-';
    data[20] = '-';
    if (data.Next(0,'*') != 0 )
        return 1;
    if (data.Next(0,'-') != 10 )
        return 2;
    if (data.Next(11,'-') != 20 )
        return 3;
    if (data.Last(49,'*') != 49 )
        return 4;
    if (data.Last(49,'-') != 20 )
        return 5;
    if (data.Last(19,'-') != 10 )
        return 6;
    return 0;
}

