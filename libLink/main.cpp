#include "elfParser.h"
#include <iostream>
#include <regex>
#include <fstream>
#include "binaryWriter.h"
#include "stdWriter.h"
using namespace std;

int main(int argc, const char *argv[])
{
    try {
        ElfFileReader f(argv[1]);
        ElfParser p(f);

        ofstream file( "out.X",ios_base::out | ios_base::binary);
        StdWriter fileObj(file);
        BinaryWriter writer(fileObj);

    } catch (string s) {
        cout << s << endl;
    } catch (const char * s) {
        cout << s << endl;
    } catch (std::regex_error e) {
        cout << e.what() << ", " << e.code()   << endl;
    }

    return 0;
}
