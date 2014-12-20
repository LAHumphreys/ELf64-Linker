#include "buildElf.h"
#include "logger.h"
#include "programHeader.h"
#include <cstring>
#include <iostream>



ElfFile::ElfFile(ElfContent& data) 
     : 
       offsets(data),
       header(data.progHeaders.size() > 0 ? 
                 ElfHeaderX86_64::NewExecutable() :
                 ElfHeaderX86_64::NewObjectFile()),
       file(1024) , 
       dataSectionStart(file),
       sectionHeadersStart(file)
{
    InitialiseHeader(data);
    InitialiseFile(data);
      
    // Process the program headers
    ProcessProgHeaders( data);

    
    sectionHeadersStart.Offset() = WriteDataSections(data);

    Bootstrap(data);

    header.SectionTableStart() = sectionHeadersStart.Offset();


    WriteSectionHeaders(data);

    // Finally write the header
    file.Writer().Write(&header,header.Size());

    // clean up any extra data
    file.Resize( (long)sectionHeadersStart + 
                 header.Sections() * header.SectionHeaderSize() );
}

void ElfFile::InitialiseFile(ElfContent& data) {
    
    long programHeadersLength =   data.progHeaders.size()
                                * header.ProgramHeaderSize();

    dataSectionStart.Offset() =   header.ProgramHeadersStart()
                                + programHeadersLength;
                         
    // Reserve space for data, plus room for alignment 
    // ( the idea is to allocate too much and resize back down )
    long sectionDataLength = 0;

    // guess the length of the sectionDataLength
    for( auto section : data.sections)
        sectionDataLength += section->Size();

    long sectionHeadersLength =   header.SectionHeaderSize()
                                * header.Sections();

    file.Resize(  dataSectionStart.Offset()
                + sectionDataLength
                + sectionHeadersLength);
    file.Fill(0,'\0',file.Size());
}

void ElfFile::InitialiseHeader(ElfContent &data) {
    
    header.ProgramHeaders() = data.progHeaders.size();
    header.Sections()  = data.sections.size();

    // If there is no load table, progheader start is defined to be 0 by 
    // the elf standard
    if ( header.ProgramHeaders() == 0 ) {
        header.ProgramHeadersStart() = 0;
    } else {
        header.ProgramHeadersStart() = header.Size();
    }
}

void ElfFile::ProcessProgHeaders(ElfContent &data ) {


    // we need a local copy to sort
    vector<ProgramHeader *> codeHeaders(data.progHeaders);

    if (codeHeaders.size() == 0 ) {
        // Some elf files have no prog. headers
        return;
    }

    auto lt = [] (ProgramHeader* lhs, ProgramHeader* rhs) -> bool {
                  int lrank = lhs->RawHeader().FileRank();
                  int rrank = rhs->RawHeader().FileRank();
                  if ( lrank == rrank ) {
                      return lhs->Address() < rhs->Address();
                  } else {
                      return lrank < rrank;
                  }
                  //Can't get here
              };
    sort(codeHeaders.begin(),codeHeaders.end(),lt);

    WriteProgHeaders(data,codeHeaders);
}

void ElfFile::WriteProgHeaders ( 
                     ElfContent& data,
                     vector<ProgramHeader *>& headers)
{
    // Track the position at the head of the file to write headers to
    BinaryWriter headerPos(file,header.ProgramHeadersStart());

    // We're responsible for writing the program headers...
    vector<ProgramHeader*>::iterator it = headers.begin();
    ProgramHeader* ph = *it;
    ph->FileSize() = headers.size() * ph->Size();


    for (;
         it != headers.end();
         ++it) {

        ph = *it;

        ph->DataStart() = offsets.AddressToOffset(ph->Address());

        headerPos << ph->RawHeader();
    }
}

BinaryWriter ElfFile::WriteDataSections( ElfContent& data)
{
    // First write
    BinaryWriter dataWritePos(dataSectionStart);
    dataWritePos.Offset() = offsets.EndOfMapped();

    for ( int i =0; i< header.Sections(); i++ ) {
        Section& sec = *(data.sections[i]);
        if (   sec.HasFileData()
        	&& !(sec.IsRelocTable() && sec.Address() ==0) )
        {
            if ( sec.IsNull() ) {
                sec.DataStart() = 0;
                SLOG_FROM (LOG_VERBOSE,
                           "ElfFile::WriteDataSections",
                           "Not Writing section (no data) : " << i)
            } else {
                if ( sec.Address() != 0 )
                {
                    sec.DataStart() = offsets.AddressToOffset(sec.Address());
                    BinaryWriter writePos(dataSectionStart);
                    writePos.Offset() = sec.DataStart();
                    sec.WriteRawData(writePos);
                    SLOG_FROM (LOG_VERBOSE,
                               "ElfFile::WriteDataSections",
                               "Writing section (at address ) : " << i)
                }
                else
                {
                    if ( sec.Alignment() != 0 )  {
                        dataWritePos.Offset() =
                            dataWritePos.NextBoundrary(sec.Alignment());
                    }
                    sec.DataStart() = dataWritePos.Offset();
                    sec.WriteRawData(dataWritePos);
                    dataWritePos.Offset() += sec.DataSize();
                    SLOG_FROM (LOG_VERBOSE,
                               "ElfFile::WriteDataSections",
                               "Writing section (tacked on end) : " << i)
                }
            }
            dataWritePos = (long)dataWritePos + sec.DataSize();
        }
    }

    // The relocation tables go on the end:
    for ( int i =0; i< header.Sections(); i++ ) {
        Section& sec = *(data.sections[i]);
        if (sec.IsRelocTable() && sec.Address() == 0)
        {
            // Align start position
            if ( sec.Alignment() != 0 )  {
                dataWritePos.Offset() = 
                    dataWritePos.NextBoundrary(sec.Alignment());
            }

            sec.DataStart() = dataWritePos;
            sec.WriteRawData(dataWritePos);
            dataWritePos.Offset() = dataWritePos + sec.DataSize();
            SLOG_FROM (LOG_VERBOSE,
                       "ElfFile::WriteDataSections",
                       "Writing reloc table: " << i)
        }
    }

    return dataWritePos;
}

bool ElfFile::IsSpecialSection(Section& s) {
    bool special = false;
    special |= s.Name() == ".shstrtab";
    special |= s.Name() == ".symtab";
    special |= s.Name() == ".strtab";
    return special;
}

void ElfFile::WriteSectionHeaders(ElfContent &data ) {
    auto writer = sectionHeadersStart;
    long idx = 0;
    // Write the standard header sections
    for ( Section* sec : data.sections ) {
        if ( ! IsSpecialSection( *sec ) ) {
            writer << (Elf64_Shdr) *sec;
            ++idx;
        }
    }

    /**
     * These have to be in a specific order at the end of the file...
     */
    WriteSpecial(data, ".shstrtab" ,idx, writer);
    WriteSpecial(data, ".symtab", idx, writer);
    WriteSpecial(data, ".strtab", idx, writer);
}

void ElfFile::WriteSpecial(ElfContent& data, string name,
                                             long& idx,
                                             BinaryWriter& writer) {
    if ( name == ".shstrtab" )  {
        this->header.StringTableIndex() = idx;
    }
    auto loc = data.sectionMap.find(name);
    if ( loc != data.sectionMap.end() ) {
        const Elf64_Shdr& sec = *data.sections[loc->second];
        writer << sec;
        ++idx;
    }
}

void ElfFile::WriteToFile(BinaryWriter& w) {
    this->file.Reader().Read(w,this->file.Size());
}

void ElfFile::Bootstrap(ElfContent& content) {
    if ( content.progHeaders.size() > 0)
    {
        Symbol* _start = content.GetSymbol("_start");
        if ( _start != nullptr) {
            header.EntryAddress() = _start->Value();
        } else {
            LOG_FROM (
                 LOG_WARNING,
                 "ElfFile::Bootstrap",
                 "Content has prog headers but no _start function!"
            )
        }
    }
}

ElfFile::SectionOffsets::SectionOffsets(const ElfContent& content) {
    FindLoadables(content);

    Elf64_Off fileOffset = 0;

    for ( auto it = loadedMap.begin(); it != loadedMap.end(); ++it )
    {
        Region& region = it->second;
        region.startOffset = fileOffset;
        region.endOffset += region.startOffset;

        // TODO: do this alignment properly, and stop hacking in an extra 4 bytes
        fileOffset = region.endOffset + 4;
    }
}


void ElfFile::SectionOffsets::FindLoadables(const ElfContent& content) {
    for (const ProgramHeader* header : content.progHeaders)
    {
        if ( header->IsLoadableSegment() )
        {
            loadedMap[header->Address()] = {
                header->Address(),
                header->AddrEnd(),
                0,
                header->FileSize()
            };
        }
    }
}

Elf64_Off ElfFile::SectionOffsets::AddressToOffset(Elf64_Addr& addr) {
    Elf64_Off offset = 0;

    for ( auto it = loadedMap.begin();
          it != loadedMap.end() && offset == 0;
          ++it )
    {
        Region& region = it->second;
        if ( region.startAddr <= addr && region.endAddr >= addr)
        {
            Elf64_Off regionOffset = addr - region.startAddr;
            offset = regionOffset + region.startOffset;
        }
    }
    return offset;
}

Elf64_Off ElfFile::SectionOffsets::EndOfMapped() {
    if ( loadedMap.size() > 0)
    {
        Region& lastRegion = loadedMap.rbegin()->second;
        return lastRegion.endOffset;
    }
    else
    {
        return 0;
    }
}
