#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
using namespace std;
int main(int argc, const char *argv[])
{
    ElfParser<ElfFileReader> p(argv[1]);
    cout << p.PrintLink() << endl;
    return 0;
}
