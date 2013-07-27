#ifndef __ENV_HEADER__
#define __ENV_HEADER__

#include <string>
using namespace std;
/*
 * Class to handle environmental variables
 *
 *
 */

class ENV {
public:
    // Return the value of the env. variable, or default
    static string GetEnvString(const string& name, const string default_value="");

    static bool   IsSet(const string& name);
private:
    /* data */
};

#endif
