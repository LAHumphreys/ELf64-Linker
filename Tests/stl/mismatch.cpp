#include <vector>
#include <algorithm>
#include "tester.h"

using namespace std;

int run();

int main(int argc, const char *argv[])
{
    Test("Check is mismatch is currupt", run).RunTest();
    return 0;
}

int run() {
    vector<int> v1(50), v2(50);
    for (int i = 0; i < 50; i++) {
        v1[i] = i;
        v2[i] = i;
    }
    mismatch(v1.begin(),v1.end(),v2.begin());
    return 0;
}
