#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
using namespace std;
int main(int argc, const char *argv[])
{
    ElfFileReader f(argv[1]);
    ElfParser p(f);
    cout << p.PrintLink() << endl;
    return 0;
}
