#include "elfParser.h"
#include "elfReader.h"
#include <iostream>
#include "buildElf.h"
#include "stdWriter.h"

using namespace std;
int main(int argc, const char *argv[])
{
    try {
        ElfFileReader f(argv[0]);
        ElfParser p(f);
        ElfContent content = p.Content();
        for ( auto s: content.sections) {
            cout << "> " << s->Name() << "< " << s->DataSize() << endl;
        }
        ElfFile file( content);
        ofstream outfile("test.out", ios_base::out | ios_base::binary);
        StdWriter fh(outfile);
        BinaryWriter w(fh);
        file.WriteToFile(w);
        fh.Flush();
        outfile.close();

    } catch (string& s) {
        cout << s << endl;
    }
    return 0;
}
