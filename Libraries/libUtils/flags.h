#include <map>
#include <string>
#ifndef FLAGS_H
   #define FLAGS_H

using namespace std;


/**
    \class   Flags
    \brief   Class to handle strings made up of single character flags
    \details  Constructed with a list of valid flags, optionally a map to human readable flags may be proivded. A maximum of 64 distinct flags may be stored
*/
class Flags {
public:
    typedef std::map<string,char> nameMap;
    // C'tor / D'tor
    Flags (const string flags, std::map<string ,char> &names);
    Flags (const string flags);
    Flags(const Flags& rhs);

    // Configure Flags object
    bool AddFlag(const char flag);
    bool AddFlag(const char flag, const string &name);
    bool AddName(const char flag, const string &name);

    // Set / unset flags
    void SetFlags(string token);
    void SetFlag(string token,bool newStatus);

    // Test flag exists
    bool operator[](const string &flag) const;
    bool operator[](char flag) const;

    // Masks
    string LinkMask();
    void FromMask(unsigned long omask) { mask = omask; }

private:
    mutable std::map<char, long> flags;
    mutable std::map<string, char> names;
    unsigned long mask;
    short flagCount;
};
#endif
