Input/Output interface (libIOInterface)
=======================================


Overview
--------

BinaryReader and BinaryWriter are very lightweight object ( a ref and a long)
which can be initialised to any object that implements the small interface they
specify

Other objects in the project use these when ever they read or write data, which
allows them to be used to write to file or some other object. BinaryReader or
BinaryWriter object represents a position in a file, and so supports
arithmetic and comparison operations

libUtils proivdes an implementation that wraps the std::fstream, arrays, and the
std::vector with this interface.

libElf defines an object for reading object files that implements  this
interface

The sub-reader object means it is trivial to treat a subsection of data as a
new file (this is used by the archive library to break apart .a files without
having to dup the data)

Reader Objects
---------------

###FileLikeReader (binaryReader.h)
A small abstract class for which defines the interface required by BinaryReader

### BinaryReader (binaryReader.h)
Represents a position in a file where data can be read from. The object is desgined for writing to specific locations in a file and must be manually repositioned after writing

Used by the elf libraries to read and write data. It requires a only a very
small interface of objects it wraps, and libUtils provides a std::vector with
this interface. Using this interface allows the library to avoid templating but
remain very versatile (with exactly the same code to parse a std::fstream object
file, a mem-mapped object file and a subection of a .a library)

-  Initialised with a FileLike object:
   <pre><code>
    ElfFileReader file("a.out");
    BinaryReader reader(file);
   </code></pre>
-  Read objects from the file. This advances the position of the reader:


   <pre><code>
    // Copy Section header structs from an elf file into an array
    sections = new Elf64_Shdr[header.Sections()];
    for( int i = 0 ; header.Sections() > i ; i++ ) {
        elfFileReader >> sections[i]; 
    }
    </code></pre>

-  Read data from a file. This is a const operation and does not
   change the position of the reader.

  + Data can be read into std::strings. Data will be read until null character
   <pre><code>
    string s1 = reader.ReadString();
    string s2 = "I'll be appended to"
    reader.AppendString(s2)
   </code></pre>

  + pre-allocated heap memory. You are responsible for marking sure there is enough space
   <pre><code>
    unsigned char * buf = new unsigned char [100];
    unsigned char line = new unsigned char [100];
    reader.Read(buf, 100) const;
    reader.ReadLine(line,100,'\n');
   </code></pre>

  + new heap memory (You are reposible for the delete )
   <pre><code>
    unsigned char * buf;
    buf = reader.Dup(100);
    // do stuff with the data then delete it
    delete [] buf;
   </code></pre>

  + A Binary Writer object
   <pre><code>
    ofstream file("a.out", "w");
    StdWriter binaryWriter(file);
    reader.Read(binaryWriter,100);
   </code></pre>

-  Move the position of this BinaryReader

   <pre><code>
    for ( int i=0; header->Sections(); ++i) {
       sections[i] = new Section(nextSection, headerStrings);
       if (sections[i]->Name() == ".symtab" ) symidx = i;
       if (sections[i]->Name() == ".strtab" ) stridx = i;
       if (sections[i]->IsLInkSection() ) ++linkSections;
       // Reposition the reader:
       nextSection += hdrSize;
    }
   </code></pre>

  + If the operand is a integer like object the position is moved.
  + If the operand is another BinaryReader the position is moved, but the
    FILE REFERENCE IS NEVER CHANGED

### SubReader (binaryReader.h)
Given any object implementing the BinaryReader interface this class represents a subsection of that data as a complete file

(for an example see libArchive)

Writer Objects
---------------

### FileLikeWriter (binaryWriter.h)
 
Small interface required for creating BinaryWriter objects


### BinaryWriter (binaryWriter.h)

Represents a position in a "file" where data can be written to. The object is desgined for writing to specific locations in a file and must be manually repositioned after writing

Used by the elf libraries to read and write data. It requires a only a very
small interface of objects it wraps, and libUtils provides a std::vector with
this interface. Using this interface allows the library to avoid templating but
remain very versatile (with exactly the same code to parse a std::fstream object
file, a mem-mapped object file and a subsection of a .a library)

-  Initialised with a FileLike object:
   <pre><code>
    ofstream file("a.out", "w");
    BinaryWriter writer(file);
   </code></pre>

Behaviour is analgous to the reader - you may write from strings, pointers to
memory or BinaryReader objects.

The << operator is used to push POD data into the file and advance the position

Arithmetic repositions the offset, but never change the file reference


### DataWriter (binaryWriter.h)

Initialised with a pointer to heap memory and a size. BinaryWriter object may
now point to the block of memory using this object as  a reference)

If a write would exceed the boundary defined by the size it was initialised
with a const char * pointing to an error message is thrown

#### DataLump (binaryWriter.h)

Used to define some data on the stack ( a local array) and provides a
FileLikeWriter interface. Templated on the array size
