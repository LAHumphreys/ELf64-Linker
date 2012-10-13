#include "arc.h"
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
Archive::Archive(const BinaryPosition& file) {
    headerStringFormat = string("!<arch>\n");
    items =0;
    if (!ValidateFile(file))
        throw "Invalid archive file";

    SimpleBinaryPosition nextMember = file + 
                                      headerStringFormat.size();
    while (nextMember < file.Reader().End()) {
        ++items;
        Archive::Member* member = new Archive::Member(nextMember);
        members.insert(members.end(),member);
        namemap[member->Name()] = member;
        nextMember += member->Size();
        // Data is 2 byte aligned
        nextMember += nextMember.Offset() %2; 
    }
}

bool Archive::ValidateFile(const BinaryPosition &file) const {
    char *fileHeader = (char *) file.Dup(headerStringFormat.size());
    bool valid = (headerStringFormat == string(fileHeader));
    delete fileHeader;
    return valid;
}

Archive::~Archive() {
    for ( auto p: members) delete p;
}

const Archive::Member& Archive::operator[] (long idx) const {
    return *(members[idx]);
}

const Archive::Member& Archive::operator[](const string& name)const
{
    auto item = namemap.find(name);
    if ( item != namemap.end() ) 
        return *(item->second);
    else
        throw "Invalid index of archive: " + name;
}

Archive::Member::Member(const BinaryPosition& p): file(NULL) {
    p.Read(&header,sizeof(header));
    file = new SubReader(p, Size());

    char sizebuf[11];
    sizebuf[11] = name[16] = '\0';
    for (int i=0; i<16; i++) name[i] =  header.name[i];
    for (int i=0; i<11; i++) sizebuf[i] =  header.size[i];
    fileSize=atol(sizebuf);
}

Archive::Member::~Member() {
    if (file) delete file;
}
