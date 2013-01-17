#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"
#include <sstream>
#include "tester.h"

using namespace std;
int main(int argc, const char *argv[])
{
        ElfFileReader f("../elf2elf/isYes/isYes.o");

        ElfParser p(f);

        //parse the input file
        ElfContent content = p.Content();
        ElfFile file( content);

		// build a binary writer to dump file to
        ofstream outfile("../elf2elf/isYes/isYes.o.test", ios_base::out | ios_base::binary);
        StdWriter fh(outfile);
        BinaryWriter w(fh);
		
		// Write out the data
        file.WriteToFile(w);

        fh.Flush();
        outfile.close();
    return 0;
}
