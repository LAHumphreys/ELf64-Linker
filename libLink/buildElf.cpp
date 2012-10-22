#include "buildElf.h"
#include "programHeader.h"
#include <cstring>


ElfFile::ElfFile(ElfContent& data) 
     : file(1024) , 
       programHeadersStart(file),
       dataSectionStart(file),
       sectionHeadersStart(file)

{
    InitialiseFile(data);
    InitialiseHeader(data);
      
    // Process the program headers
    sectionHeadersStart = (long)ProcessProgHeaders( data);
    header.e_shoff = sectionHeadersStart;

    WriteSectionHeaders(data);

    // Finally write the header
    file.Writer().Write(&header,header.e_ehsize);

    // clean up any extra data
    file.resize( (long)sectionHeadersStart + 
                 header.e_shnum * header.e_shentsize );
}

void ElfFile::InitialiseFile(ElfContent& data) {
    long sectionDataLength = 0;
    
    programHeadersStart = data.header.Size();
    dataSectionStart = (long) programHeadersStart + 
                       (  data.header.ProgramHeaderSize() 
                        * data.progHeaders.size() );
    // Reserve space for data, plus room for alignment 
    // ( the idea is to allocate too much and resize back down )

    // guess the length of the sectionDataLength
    for( auto section : data.sections)
        sectionDataLength += section->Size();
    // allow room for alignments of loadable segments
    sectionDataLength += 16 * header.e_phnum;

    file.resize(  (long) dataSectionStart
                + sectionDataLength
                + header.e_shentsize * header.e_shnum);
}

void ElfFile::InitialiseHeader(ElfContent &data) {
    // we had nothing to do with formating the data:
    data.header.GetHeader(this->header);

    this->header.e_phnum = data.progHeaders.size();
    this->header.e_shnum = data.sections.size();
    
    this->header.e_phoff = sizeof(this->header);
    // We can't set the 
}


BinaryWriter ElfFile::ProcessProgHeaders(ElfContent &data ) {
    // we need a local copy to sort
    auto headers = data.progHeaders;

    BinaryWriter dataWritePos = dataSectionStart;
    auto dataEnd = dataWritePos;
    auto pheaderWritePos = programHeadersStart;

    headers.reserve(headers.size());

    SortByAddress(headers.begin(),headers.end());

    for ( auto ph: headers ) {
        // We need to align the program segment: (see comment in .h)
        // Calculate boundrary location
        dataEnd = (long)dataWritePos.NextBoundrary( ph->Alignment()) 
                + (ph->Address() % ph->Alignment());
        // move to the boundrary
        dataWritePos.FillTo(dataEnd);
        dataWritePos = (long)dataEnd;

        // write the section data
        dataEnd = (long)WriteDataSections( data, *ph, dataWritePos);

        // Set the file position in the program header
        ph->DataStart() = dataWritePos;
        ph->FileSize() = dataEnd - dataWritePos;

        pheaderWritePos.Write(ph,header.e_phentsize);
        pheaderWritePos += header.e_phentsize;
    }
}


BinaryWriter ElfFile::WriteDataSections( ElfContent &data, 
                                         ProgramHeader& prog,
                                         BinaryWriter& writer) {
    BinaryWriter end = writer;
    BinaryWriter writePos = writer;
    Section * section;
    for ( auto sname: prog.SectionNames() ) {
        section = data.sections[data.sectionMap[sname]];

        if ( prog.Address() != 0 ) {
            if ( section->Address() == 0 ) {
                string error = "FATAL ERROR: A section in a ";
                error += "loadable segment MUST specify a load ";
                error += "address. Segment table is currupt, I ";
                error += "cannot continue";
                throw error;
            }
            writePos = (long)writer + section->Address() 
                                    - prog.Address();
            section->WriteRawData(writePos);
            if (end <=  section->Size() + writePos)
                 end =  section->Size() + writePos;

        } else {
            writePos = (long)end;
            section->DataStart() = writePos;
            section->WriteRawData(writePos);
            // These are not loadable, we have no responsibility to 
            // align them
            end = section->Size() + writePos;
        }
    }
}

bool ElfFile::IsSpecialSection(Section& s) {
    bool special = false;
    special |= s.Name() == ".shstratab";
    special |= s.Name() == ".symtab";
    special |= s.Name() == ".strtab";
    return special;
}

void ElfFile::WriteSectionHeaders(ElfContent &data ) {
    auto writer = dataSectionStart;
    long idx = 0;
    // Write the standard header sections
    for ( auto sec : data.sections ) {
        if ( ! IsSpecialSection( *sec ) ) {
            const auto& sheader = sec->RawHeader();
            writer.Write(&sheader,header.e_shentsize);
            writer += header.e_shentsize;
            ++idx;
        }
    }
    for ( auto s: {".shstrtab", ".symtab", ".symtab" } ) {
        if ( s == ".shstrtab" )
            this->header.e_shstrndx = idx;
        auto loc = data.sectionMap.find(s);
        if ( loc != data.sectionMap.end() ) {
            auto sec = data.sections[loc->second];
            const auto& sheader = sec->RawHeader();
            writer.Write(&sheader,header.e_shentsize);
            writer += header.e_shentsize;
            ++idx;
        }
    }
}
