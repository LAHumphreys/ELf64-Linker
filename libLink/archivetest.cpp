#include <iostream>
#include "arc.h"
#include "elfReader.h"
#include "binaryReader.h"

using namespace std;

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "usage: archiveTest <fileName>" << endl;
        exit(1);
    }
    ElfFileReader file(argv[1]);
    Archive arc = Archive(BinaryReader(file));
    for (long i=0; i< arc.Count(); i++ ) {
        cout << arc[i].Name() << ": " << arc[i].Size() << endl;
    }
    return 0;
}
