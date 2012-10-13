#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include <map>
#include <string>
#include "binaryReader.h"

using namespace std;

class Archive {
public:

    struct MemberHeader {
        char name[16];
        char modtime[12];
        char uid[6];
        char gid[6];
        char mode[8];
        char size[10];
        char eol[2];
    };

    class Member {
    public:
        Member (const BinaryPosition &p);
        virtual ~Member();
        string Name() const { return string(header.name); }
        long   Size() const { return atol(header.size); }
        const BinaryReader& File() const { return *file; }
    private:
        Archive::MemberHeader header;
        SubReader *file;
    };

    Archive (const BinaryPosition& file);
    const Archive::Member& operator[](int idx) const;
    const Archive::Member& operator[](const string& name) const;
    bool ValidateFile(const BinaryPosition &file) const ;
    long Count() const { return items; }
    virtual ~Archive ();

private:
    string headerStringFormat;
    std::vector<Archive::Member *> members;
    std::map<const string, Member *> namemap;
    int items;
};

#endif
