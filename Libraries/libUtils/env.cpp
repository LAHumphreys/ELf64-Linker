#include "env.h"
#include <cstdlib>

using namespace std;
string ENV::GetEnvString(const string& name, const string default_value) {
    const char* value = getenv(name.c_str());

    if ( value != NULL ) {
        return string(value);
    } else {
        return default_value;
    }
}

bool ENV::IsSet(const string& name) {
    return getenv(name.c_str()) != NULL;
}
