#include "buildElf.h"
#include <cstring>

ElfFile::MakeNewHeader(ElfContent &data) {
    // we had nothing to do with formating the data:
    this->header.GetHeader(this->header);

    this->header.ProgramHeaders() = data.progHeaders.size();
    this->header.Sections() = data.sections.size();
}

void ElfFile
