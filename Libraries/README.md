Libaries Folder
===============
This is a single folder to contain c++ libraries used in the project:

1.  libArchive
    Small libraries for reading .a files
2.  libIOInterface/
    Small library which defines an interface for reading and wrting to an
    object. Othe libraries use this interface so they are blind as to whether
    they are reading to a std::fstream, a mem-mapped file or even a std::vector
    (see libUtils)
3.  libElf
    Contains classes for reading and writing linux ELF files
4.  libLINK
    Contains classes for reading from LINK files
5.  libUtils
    Helper objects used by other libraries / binaries
