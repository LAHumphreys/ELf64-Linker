
#ifndef _EXEC_BUILDER_H
#define _EXEC_BUILDER_H

#include <string>

// Members
#include "buildElf.h"
#include "binaryReader.h"
#include "binaryWriter.h"

class FileLikeObject;


/* 
 * Utility class to make inplace changes to 
 * an executable ELF file.
 *
 * Each function is a complete action, that leave the 
 * source binary in a suitable state to be executed, and 
 * the values in ElfContent correct
 *
 *
 *
 */

class ExecBuilder {
public:
    ExecBuilder(FileLikeObject& f, ElfContent& c);
    ExecBuilder(FileLikeObject&& f, ElfContent&& c)
         : ExecBuilder(f,c){}


    void MakeDataExecutable();

    // Returns the starting address of the new memorry
    long InflateData(long newAllocation);

    // Update the value of an existing symbol in a file
    bool UpdateSymbolValue(string name, long value);


private:
    ExecBuilder();
    ExecBuilder(ExecBuilder& other);
    BinaryReader reader;
    BinaryWriter writer;
    ElfContent content; // struct of refs
    Section& symTabHeader;
};

#endif
