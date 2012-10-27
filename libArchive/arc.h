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
        Member (const BinaryReader &p);
        virtual ~Member();
        string Name() const { return string(name); }
        long Size() const { 
            return fileSize + sizeof(header);
        }
        long  FileSize() const { return fileSize; }
        const BinaryReader File() const { 
            return file->Begin(); 
        }
    private:
        Archive::MemberHeader header;
        char name[17];
        long fileSize;
        SubReader *file;
    };

    Archive (const BinaryReader& file);
    const Archive::Member& operator[](long idx) const;
    const Archive::Member& operator[](const string& name) const;
    bool ValidateFile(const BinaryReader &file) const ;
    long Count() const { return items; }
    virtual ~Archive ();

private:
    string headerStringFormat;
    std::vector<Archive::Member *> members;
    std::map<const string, Member *> namemap;
    int items;
};

#endif
