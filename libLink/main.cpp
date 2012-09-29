#include "x86Parser.h"
#include <iostream>
#include <regex>
using namespace std;

int main(int argc, const char *argv[])
{
    try {
        X86Parser p(argv[1]);
        p.Write("out.elf");
    } catch (string s) {
        cout << s << endl;
    } catch (const char * s) {
        cout << s << endl;
    } catch (std::regex_error e) {
        cout << e.what() << ", " << e.code()   << endl;
    }

    return 0;
}
