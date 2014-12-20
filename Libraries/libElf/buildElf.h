#ifndef BUILD_ELF_H
#define BUILD_ELF_H

#include "binaryReader.h"
#include "binaryWriter.h"
#include "dataVector.h"
#include <vector>
#include <map>
#include <algorithm>
#include "elf.h"
#include "elfHeader.h"
#include "programHeader.h"
#include "section.h"
#include "symbol.h"

struct ElfContent {
    Section* GetSection(string name) {
        auto loc = sectionMap.find(name);
        if ( loc != sectionMap.end() ) {
            return sections[loc->second];
        } else {
            return NULL;
        }
    }
    Symbol* GetSymbol(string name) {
        auto loc = symbolMap.find(name);
        if ( loc != symbolMap.end() ) {
            return symbols[loc->second];
        } else {
            return NULL;
        }
    }
    ElfHeaderX86_64 &header;
    std::vector<Section *>& sections;
    std::vector<ProgramHeader *>& progHeaders;
    std::vector<Symbol *>& symbols;
    std::map<string, int>& sectionMap;
    std::map<string, int>& symbolMap;
};

class ElfFile{
public:
    ElfFile(ElfContent &data);
    ElfFile(ElfContent &&data): ElfFile(data){}
    void MakeNewHeader(ElfContent &data);
    void ProcessProgHeaders(ElfContent& data);
    void WriteSectionHeaders(ElfContent& data);
    void WriteToFile(BinaryWriter& w);
    inline void WriteToFile(BinaryWriter&& w) { WriteToFile(w); }
protected:
    void InitialiseFile(ElfContent& data);
    void InitialiseHeader(ElfContent& data);
    bool IsSpecialSection(Section& s);
    void WriteSpecial(ElfContent&, string name, long&, BinaryWriter&);

    /**
     * Write the program headers
     *
     * @param data        The raw-data supplied to the c'tor
     * @param headers     A list of program headers, in the order in whih they
     *                    should be written
     */
    void WriteProgHeaders ( ElfContent& data,
    		                vector<ProgramHeader *>& headers);

    /**
     * If the elf file has been configured to be an executable, set the start
     * address to be the location of the _start function.
     *
     * @param content  The raw file-data
     */
    void Bootstrap(ElfContent& content);

    BinaryWriter WriteDataSections( ElfContent& data);
private:
    class SectionOffsets {
    public:
    	SectionOffsets(const ElfContent& content);

    	Elf64_Off AddressToOffset(Elf64_Addr& addr);

    	Elf64_Off EndOfMapped();
    private:

    	void FindLoadables(const ElfContent& content);

    	struct Region {
    		Elf64_Addr  startAddr;
    		Elf64_Addr  endAddr;
    		Elf64_Off   startOffset;
    		Elf64_Off   endOffset;
    	};

    	std::map<Elf64_Addr,Region>    loadedMap;

    } offsets;

    ElfHeaderX86_64 header;

    //final data
    DataVector file;

    // utility data
    long sectionDataLength;
    BinaryWriter dataSectionStart;
    BinaryWriter sectionHeadersStart;
};


/*
 * We're trying to build this:
 *
 *  +-------------------------------+
 *  | ELF File Header               |
 *  +-------------------------------+
 *  | Program Header for segment #1 |
 *  +-------------------------------+
 *  | Program Header for segment #2 |
 *  +-------------------------------+
 *  | ...                           |
 *  +-------------------------------+
 *  | Contents (Byte Stream)        |
 *  | ...                           |
 *  +-------------------------------+
 *  | Section Header for section #1 |
 *  +-------------------------------+
 *  | Section Header for section #2 |
 *  +-------------------------------+
 *  | ...                           |
 *  +-------------------------------+
 *  | ".shstrtab" section header    |
 *  +-------------------------------+
 *  | ".symtab"   section header    |
 *  +-------------------------------+
 *  | ".strtab"   section header    |
 *  +-------------------------------+
 *  
 */

/*
 *                The Elf Header
 *              +----------------+
 *  The class will build up a new ELfHeader from scratch
 *
 */

/* 
 *            Program headers and Section headers
 *          +-------------------------------------+
 *
 * Program headers will be written in the order in which they appear
 * in the program headers array. 
 *
 * In the absence of program headers, section headers (and their 
 * data will be written in the order in which they appear in the
 * input array, although the final 3 will always be, if included 
 * in the input, .shstrtab, then .symtab and finally .strtab.
 *   If program headers are provided, then sections are written as 
 * they are mentioned by the program headers, with section that 
 * define a v_addr being written at the correct relative address 
 * to the start of the program header. It is the callers 
 * responsibility to ensure this isn't a stupid memorry location. 
 *
 * +------------------------+--------------------+
 * |              ELF Program Header             |
 * +------------------------+--------------------+
 * | Callers Responsibility | Our Responsibility |
 * +------------------------+--------------------+
 * | p_type                 | p_offset           |
 * +------------------------+--------------------+
 * | p_flags                | p_filesz           |
 * +------------------------+--------------------+
 * | p_vaddr                |                    |
 * +------------------------+--------------------+
 * | p_paddr                |                    |
 * +------------------------+--------------------+
 * | p_memsz                |                    |
 * +------------------------+--------------------+
 * | p_align                |                    |
 * +------------------------+--------------------+
 *
 * +------------------------+--------------------+
 * |              ELF Section Header             |
 * +------------------------+--------------------+
 * | Callers Responsibility | Our Responsibility |
 * +------------------------+--------------------+
 * | sh_name                | sh_offset          |
 * +------------------------+--------------------+
 * | sh_type                | sh_link(=0)        |
 * +------------------------+--------------------+
 * | sh_flags               |                    |
 * +------------------------+--------------------+
 * | sh_addr                |                    |
 * +------------------------+--------------------+
 * | sh_size                |                    |
 * +------------------------+--------------------+
 * | sh_info                |                    |
 * +------------------------+--------------------+
 * | sh_addralign           |                    |
 * +------------------------+--------------------+
 * | sh_entsize             |                    |
 * +------------------------+--------------------+
 *
 *
 *  Aligning Data
 * +-------------+
 * -> Loadable segments must be aligned to p_align value in the 
 *    header file section: p_vaddr % p_align = p_offset % p_align. 
 *    This will be handled here
 * -> Now it is required that: sh_addr % sh_addralign = 0. However 
 *    the caller must handle this, as changing anything here would 
 *    require re-locating symbols
 */
#endif
