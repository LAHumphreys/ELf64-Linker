using namespace std;
#include <cstring>
#include <ctime>
#include <iostream>
#include "dataLump.h"
#include "tester.h"
#include "fileLikeObject.h"
#include <sstream>

long Arrays(testLogger& log);
long ArraysStack (testLogger& log);
long Inline(testLogger& log);
long InlineHeap (testLogger& log);
long Vtable (testLogger& log);
long heapVtable (testLogger& log);
long viaVtable (FileLikeObject& arr2s, FileLikeObject& arr5s, 
                                       FileLikeObject& both,
                                       testLogger& log);

const long MB = 1024*1024;
const long maxCount = 0.0099*MB;
//WARNING!: arbitrary - make sure this is compatible with your setup
const long STACK_CUTOFF = 0.01*MB; 

//Uncoment this to dump arrays if something goes wrong
//#define DEBUG
int main(int argc, const char *argv[])
{
    cout << "Validating dataLump object.." << endl;
    testLogger log;
    clock_t startTime, endTime;
    long result;
    double time;
    bool ret = true;
    
    cout << "  Using raw arrays on the heap: ";
    startTime = clock();
    result = Arrays(log);
    endTime = clock();
    time = (double(endTime)  - double(startTime)) 
                  / CLOCKS_PER_SEC; cout << time << "s" << endl;

    if ( maxCount < STACK_CUTOFF ) {
        cout << "  Using raw arrays on the stack: ";
        startTime = clock();
        result = ArraysStack(log);
        endTime = clock();
        time = (double(endTime)  - double(startTime)) 
                      / CLOCKS_PER_SEC;
        cout << time << "s" << endl;
    }

    cout << "  Using dataLump on heap: ";
    startTime = clock();
    if ( InlineHeap(log) != result ) ret =false;
    endTime = clock();
    time = (double(endTime)  - double(startTime)) 
                  / CLOCKS_PER_SEC;
    cout << time << "s" << endl;

    if ( maxCount < STACK_CUTOFF ) {
        cout << "  Using dataLump on the stack: ";
        startTime = clock();
        if ( Inline(log) != result ) ret = false;
        endTime = clock();
        time = (double(endTime)  - double(startTime)) 
                      / CLOCKS_PER_SEC;
        cout << time << "s" << endl;
    }

    cout << "  Using virtual interface (heap): ";
    startTime = clock();
    if ( heapVtable(log) != result ) ret = false;
    endTime = clock();
    time = (double(endTime)  - double(startTime)) 
                  / CLOCKS_PER_SEC;
    cout << time << "s" << endl;


    if ( maxCount < STACK_CUTOFF ) {
        cout << "  Using virtual interface (stack): ";
        startTime = clock();
        if ( Vtable(log) != result ) ret = false;
        endTime = clock();
        time = (double(endTime)  - double(startTime)) 
                      / CLOCKS_PER_SEC;
        cout << time << "s" << endl;
    }

    if ( ret ) {
        cout << "TEST PASSED" << endl;
    } else {
        cout << "TEST FAILED, log follows" << endl;
        cout << log.str() << endl;
        return 1;
    }
    return 0;
}

long heapVtable (testLogger& log) {
    log << "3. Using virtual base class resolving to heap memory" << endl;
    DataLump<maxCount> *parr2s = new DataLump<maxCount>;
    DataLump<maxCount> *parr5s = new DataLump<maxCount>;
    DataLump<maxCount> *pboth  = new DataLump<maxCount>;
    long ret = viaVtable(*parr2s, *parr5s, *pboth, log);
    delete parr2s;
    delete parr5s;
    delete pboth;
    return ret;
}

long Vtable (testLogger& log) {
    log << "5. Using virtual base class resolving to heap memory" << endl;
    DataLump<maxCount> arr2s;
    DataLump<maxCount> arr5s;
    DataLump<maxCount> both;
    long ret = viaVtable(arr2s, arr5s, both, log);
    return ret;
}

long ArraysStack (testLogger& log) {
    log << "1. Using raw arrays: " << endl;
    double totTime = 0;
    clock_t startTime, endTime;
    startTime = clock();
    unsigned char arr2s[maxCount];
    unsigned char arr5s[maxCount];
    unsigned char both[maxCount];
    endTime = clock();
    double time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to declare arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    memset(arr2s,0,maxCount);
    memset(arr5s,0,maxCount);
    memset(both,0,maxCount);
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to zero arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    long i;
    for (i = 1; i < maxCount + 1; i++) {
        if ( i % 2 == 0 ) {
            arr2s[i-1] = 1;
        }
        if ( i % 5 == 0 ) {
            arr5s[i-1] = 1;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to fill arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if ( arr5s[i-1] && arr2s[i-1]) {
            both[i-1] = 1;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to comp arrays: " << time << endl;
    totTime += time;


#ifdef DEBUG
    for (i = 1; i < maxCount + 1; i++) {
        log << i << ": " << arr2s[i-1] << " , " << arr5s[i-1] << " , " << both[i-1] << endl;
    }
#endif

    unsigned long result = 0;
    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if (both [i-1] ) {
            result++;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to count array: " << time << endl;
    log << "Result: " << result << endl;
    totTime+= time;
    log << "Total time: " << totTime << endl;;
    return result;
}

long Arrays (testLogger& log) {
    log << "1. Using raw arrays: " << endl;
    double totTime = 0;
    clock_t startTime, endTime;
    startTime = clock();
    unsigned char * arr2s = new unsigned char [maxCount];
    unsigned char * arr5s = new unsigned char [maxCount];
    unsigned char * both = new unsigned char [maxCount];
    endTime = clock();
    double time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to declare arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    memset(arr2s,0,maxCount);
    memset(arr5s,0,maxCount);
    memset(both,0,maxCount);
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to zero arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    long i;
    for (i = 1; i < maxCount + 1; i++) {
        if ( i % 2 == 0 ) {
            arr2s[i-1] = 1;
        }
        if ( i % 5 == 0 ) {
            arr5s[i-1] = 1;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to fill arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if ( arr5s[i-1] && arr2s[i-1]) {
            both[i-1] = 1;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to comp arrays: " << time << endl;
    totTime += time;


#ifdef DEBUG
    for (i = 1; i < maxCount + 1; i++) {
        log << i << ": " << arr2s[i-1] << " , " << arr5s[i-1] << " , " << both[i-1] << endl;
    }
#endif

    unsigned long result = 0;
    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if (both [i-1] ) {
            result++;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to count array: " << time << endl;
    log << "Result: " << result << endl;
    totTime+= time;
    log << "Total time: " << totTime << endl;;
    delete [] arr2s;
    delete [] arr5s;
    delete [] both;
    return result;
}

long InlineHeap (testLogger& log) {
    log << "2. Using naked class (on the heap) with inlines: " << endl;
    double totTime = 0;
    clock_t startTime, endTime;
    startTime = clock();
    DataLump<maxCount> *parr2s = new DataLump<maxCount>;
    DataLump<maxCount> *parr5s = new DataLump<maxCount>;
    DataLump<maxCount> *pboth  = new DataLump<maxCount>;
    endTime = clock();
    double time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to declare arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    parr2s->Fill(0,0,maxCount);
    parr5s->Fill(0,0,maxCount);
    pboth->Fill(0,0,maxCount);
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to zero arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    long i;
    for (i = 1; i < maxCount + 1; i++) {
        if ( i % 2 == 0 ) {
            parr2s->Put(i-1,1);
        }
        if ( i % 5 == 0 ) {
            parr5s->Put(i-1,1);
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to fill arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if ( parr5s->Get(i-1) && parr2s->Get(i-1)) {
            pboth->Put(i-1,1);
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to comp arrays: " << time << endl;
    totTime += time;
#ifdef DEBUG
    for (i-1 = 1; i < maxCount + 1; i++) {
        log << i << ": " << parr2s->Get(i-1) << " , " << parr5s->Get(i-1) << " , " << pboth->Get(i-1) << endl;
    }
#endif

    unsigned long result = 0;
    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if (pboth->Get(i-1) ) {
            result++;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to count array: " << time << endl;
    log << "Result: " << result << endl;
    totTime += time;
    log << "Total time: " << totTime << endl;


    delete parr2s;
    delete parr5s;
    delete pboth;
    return result;
}

long Inline (testLogger& log) {
    log << "4. Using naked class (on the stack) with inlines: " << endl;
    double totTime = 0;
    clock_t startTime, endTime;
    startTime = clock();
    DataLump<maxCount> arr2s;
    DataLump<maxCount> arr5s;
    DataLump<maxCount> both;
    endTime = clock();
    double time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to declare arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    arr2s.Fill(0,0,maxCount);
    arr5s.Fill(0,0,maxCount);
    both.Fill(0,0,maxCount);
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to zero arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    long i;
    for (i = 1; i < maxCount + 1; i++) {
        if ( i % 2 == 0 ) {
            arr2s.Put(i-1,1);
        }
        if ( i % 5 == 0 ) {
            arr5s.Put(i-1,1);
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to fill arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if ( arr5s.Get(i-1) && arr2s.Get(i-1)) {
            both.Put(i-1,1);
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to comp arrays: " << time << endl;
    totTime += time;
#ifdef DEBUG
    for (i-1 = 1; i < maxCount + 1; i++) {
        log << i << ": " << arr2s.Get(i-1) << " , " << arr5s.Get(i-1) << " , " << both.Get(i-1) << endl;
    }
#endif

    unsigned long result = 0;
    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if (both.Get(i-1) ) {
            result++;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to count array: " << time << endl;
    log << "Result: " << result << endl;
    totTime += time;
    log << "Total time: " << totTime << endl;
    return result;
}

long viaVtable (FileLikeObject& arr2s, FileLikeObject& arr5s, 
                                       FileLikeObject& both,
                                       testLogger& log) {
    clock_t startTime, endTime;
    double time;
    double totTime = 0;

    startTime = clock();
    arr2s.Fill(0,0,maxCount);
    arr5s.Fill(0,0,maxCount);
    both.Fill(0,0,maxCount);
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to zero arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    long i;
    for (i = 1; i < maxCount + 1; i++) {
        if ( i % 2 == 0 ) {
            arr2s.Put(i-1,1);
        }
        if ( i % 5 == 0 ) {
            arr5s.Put(i-1,1);
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to fill arrays: " << time << endl;
    totTime += time;

    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if ( arr5s.Get(i-1) && arr2s.Get(i-1)) {
            both.Put(i-1,1);
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to comp arrays: " << time << endl;
    totTime += time;
#ifdef DEBUG
    for (i-1 = 1; i < maxCount + 1; i++) {
        log << i << ": " << arr2s.Get(i-1) << " , " << arr5s.Get(i-1) << " , " << both.Get(i-1) << endl;
    }
#endif

    unsigned long result = 0;
    startTime = clock();
    for (i = 1; i < maxCount + 1; i++) {
        if (both.Get(i-1) ) {
            result++;
        }
    }
    endTime = clock();
    time = (double(endTime)  - double(startTime)) / CLOCKS_PER_SEC;
    log << "Time to count array: " << time << endl;
    log << "Result: " << result << endl;
    totTime += time;
    log << "Total time: " << totTime << endl;
    return result;
}
