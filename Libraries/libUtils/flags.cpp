#ifndef FLAGS_H
   #include "flags.h"
#endif
#include <sstream>
#include <iostream>
Flags::Flags (string flags) {
    flagCount=0;
    for (auto flag: flags ) {
        AddFlag(flag);
        //cout << "processing" << flag << endl;
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

Flags::Flags(const Flags& rhs):
    flagCount(rhs.flagCount),
    mask(rhs.mask),
    names(rhs.names),
    flags(rhs.flags) 
{ 
}

Flags::Mask Flags::AddFlag(const char flag) {
    if  (flagCount < 63) {
        Flags::Mask base = 0x1;
        ++flagCount;
        return flags[flag] = base << flagCount;
    } else {
        return 0;
    }
}

Flags::Mask Flags::AddFlag(const char flag, const string &name) {
    const Flags::Mask& ret = AddFlag(flag);
    if  (ret) {
        AddName(flag,name);
    }
    return ret;
}

void Flags::AddName(const char flag, const string &name ) {
    names[name] = flag;
}

bool Flags::operator[] (char flag) const {
    return flags[flag] & mask;
}

bool Flags::operator[] (const string &flag) const {
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
    auto it = names.find(flag);
    if ( it != names.end() ) {
        cflag = it->second;
    } else {
        cflag = flag[0];
    }
    if ( it != names.end() ) {
        auto flagMask = flags[cflag];
        if ( newStatus ) {
            // Turn on this bit
            mask |= flagMask;
        } else {
            // switch off this bit
            mask &= ~flagMask;
        }
    }
}

string Flags::LinkMask() {
    ostringstream mask;
    for (auto flag: flags) {
        if (this->operator[](flag.first)) mask << flag.first;
    }
    return mask.str();
}
