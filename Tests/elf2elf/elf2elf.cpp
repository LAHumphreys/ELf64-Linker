#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
using namespace std;
int main(int argc, const char *argv[])
{
    return 0;
    ElfFileReader f(argv[0]);
    ElfParser p(f);
    cout << p.PrintLink() << endl;
    return 0;
}
