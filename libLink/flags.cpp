#ifndef FLAGS_H
   #include "flags.h"
#endif
#include <string>
Flags::Flags (string flags) {
    flagCount=0;
    for (auto flag: flags ) {
        AddFlag(flag);
    }
}
Flags::Flags (string flags, std::map<string,char> &nameMap)
    :/*Flags(flags),*/ names(nameMap)
{
    // TODO: delete this code and use c++11 delegated constructor
    // when gcc 4.7 is available on ubuntu next month...
    flagCount=0;
    for (auto flag: flags ) {
        AddFlag(flag);
    }

}

bool Flags::AddFlag(const char flag) {
    if  (flagCount < 63) {
        long base = 0x1;
        flags[flag] = base << flagCount;
        ++flagCount;
        return true;
    } else {
        return false;
    }
}

bool Flags::AddFlag(const char flag, const string &name) {
    if  (AddFlag(flag)) {
        return AddName(flag,name);
    } else {
        return false;
    }
}

bool Flags::AddName(const char flag, const string &name ) {
    names[name] = flag;
    return true;
}

bool Flags::operator[] (char flag) {
    return flags[flag] & mask;
}
bool Flags::operator[] (const string &flag) {
    char cflag;
    if ( names.count(flag) ) {
        cflag = names[flag];
    } else { 
        cflag = names[0];
    }
    auto flagmask = flags[cflag];
    return mask & flagmask;
}

void Flags::SetFlags(string token) {
    for (auto flag: token) {
        if ( flags.count(flag)) {
            mask |= flags[flag];
        }
    }
}

void Flags::SetFlag(string flag, bool newStatus) {
    char cflag;
    if ( names.count(flag) ) {
        cflag = names[flag];
    } else {
        cflag = flag[0];
    }
    if ( flags.count(cflag)) {
        auto flagMask = flags[cflag];
        if ( newStatus ) {
            // Turn on this bit
            mask |= flagMask;
        } else {
            // switch off this bit
            auto invertedMask = ~flagMask;
            mask &=invertedMask;
        }
    }
}

string Flags::LinkMask() {
    string mask = "";
    for (auto flag: flags) {
        if (this->operator[](flag.first)) mask+=flag.second;
    }
    return mask;
}
