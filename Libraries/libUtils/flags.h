#include <map>
#include <string>
#include <memory>
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
    typedef unsigned long Mask;
    static const Mask EmptyMask = 0x0;
    static const short maxItems = 8 * sizeof(Mask);


    // C'tor / D'tor
    Flags (const string flags, std::map<string ,char> &names);
    Flags (const string flags);
    Flags(const Flags& rhs); 
    // Configure Flags object
    Mask AddFlag(const char flag);
    Mask AddFlag(const char flag, const string &name);
    void AddName(const char flag, const string &name);

    // Set / unset flags
    void SetFlags(string token);
    void SetFlag(string token,bool newStatus);

    // Turn on or off a subset of flags
    inline void SetFlags(const Mask& flagMask,bool newStatus) {
        if (newStatus) {
            // Turn on the bit(s)
            mask |= flagMask;
        } else {
            // Turn off the bit(s)
            mask &= ~flagMask;
        }
    }
    // Set the flags
    inline void SetFlags(const Mask& flagMask) {
        mask = flagMask;
    }

    // Test flag exists
    bool operator[](const string &flag) const;
    bool operator[](char flag) const;

    inline Mask operator[](const Mask& flagMask) const {
        return flagMask & this->mask;
    }

    // Masks
    string LinkMask();
    void FromMask(const Mask& omask) { mask = omask; }

protected:
    typedef std::map<string,char> NameMap;
    typedef std::map<char,Mask> MaskMap;

private:
    shared_ptr<MaskMap> flags;
    shared_ptr<NameMap> names;
    unsigned long mask;
    short flagCount;
};
#endif
