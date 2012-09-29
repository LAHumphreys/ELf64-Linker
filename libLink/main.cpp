#include "reader.h"
#include "elfHeader.h"
#include <iostream>
#include <regex>
using namespace std;

int main(int argc, const char *argv[])
{
    try {
        LinkReader r(argv[1]);
        ElfHeaderX86_64 h("1 2 3");
        r.test();
    } catch (string s) {
        cout << s << endl;
    } catch (std::regex_error e) {
        cout << e.what() << ", " << e.code()   << endl;
    }

    return 0;
}
