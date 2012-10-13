#include "arc.h"
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

const Archive::Member& Archive::operator[] (int idx) const {
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
}

Archive::Member::~Member() {
    if (file) delete file;
}
