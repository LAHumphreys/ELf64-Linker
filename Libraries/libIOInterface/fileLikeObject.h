#ifndef FILE_LIKE_OBJECT
#define FILE_LIKE_OBJECT
#include "binaryReader.h"
#include "binaryWriter.h"

// pure virtual class that defines both interfaces
class FileLikeObject : virtual public FileLikeWriter, virtual public FileLikeReader {
};

#endif
