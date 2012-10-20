#include "mymath.h"
char str[] = "Hello world";
int main(int argc, const char *argv[])
{
        int a, b;
        float c;
        int i = someglobal;
        int *p = &someglobal;
        char *cp = str +3;
        char *cm = str -300000000;
        long j = (long) someglobal;
        // This time we actually want to do some linking...
        a = adder(a,b);
        b = radder(&a,&b);
        return 0;
}
