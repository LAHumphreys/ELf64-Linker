#ifndef ELF_ARCHIVE_H
#define ELF_ARCHIVE_H
#include "arc.h"


class ElfArchive {
public:
    ElfArchive (const BinaryReader& p);
    virtual ~ElfArchive ();

private:
    Archive rawArchive;
};

#ifndef
