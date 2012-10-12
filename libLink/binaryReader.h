#include <string>
#ifndef BINARY_READER_H
#define BINARY_READER_H

using namespace std;

class BinaryPosition; //forward declaration
class SimpleBinaryPosition;

// Interface for a common base class for binary readers, see
// ElfReader for an example implementation
class BinaryReader {
    public:
        typedef BinaryPosition PositionType;
        virtual void Read(long offset, void *dest, long size) 
                                                         const =0;
        virtual void ReadString(long offset, std::string& dest) 
                                                         const =0;
        virtual SimpleBinaryPosition Begin() const =0;
        virtual SimpleBinaryPosition Pos(long offset) const =0;
};


// Interface for a binary position, a reader object should either
// be compatible with the CommonPosition object, or define a custom
// implementation
//
// We may not define a pure virtual constructor, but it is expected
// that any implementation behaves sensibly if given another
// BinaryPosition as a single constructor argument
class BinaryPosition {
    public:
        // Access data
        virtual void Read(void *dest, long size) const =0;
        virtual void ReadString(std::string& dest) const =0;
        virtual std::string ReadString() const =0;
        virtual void AppendString(std::string& dest) const =0;
        virtual unsigned char * Dup(long size) const =0; 
        // (it is the responsibility of the caller to delete this
        // memory)

        // Create a new BinaryPosition

        virtual SimpleBinaryPosition operator+
                                  (long additionalOffset) const =0;
        virtual SimpleBinaryPosition operator-
                                  (long additionalOffset) const =0;
        // Reposition the pointer
        virtual BinaryPosition& operator+=(long additionalOffset)
                                                               =0;
        virtual BinaryPosition& operator-=(long additionalOffset)
                                                               =0;
        // This is the position from the beging on the file
        virtual BinaryPosition& operator=(long position)=0;

        virtual const BinaryReader& Reader() const =0;
        virtual long Offset() const =0;
};

// Since all of the demands of BinaryPosition can be met by the
// interface required of BinaryReader, create a simple
// implementation.
class SimpleBinaryPosition: public BinaryPosition {
    public:
        friend class BinaryReader; // see below
        SimpleBinaryPosition(const BinaryReader& r, long offset);
        SimpleBinaryPosition(const BinaryPosition &other);
        // We don't care if its a temporary ref, we're only going
        // to copy data anyway
        SimpleBinaryPosition(BinaryPosition &&other);

        // Access data
        virtual void Read(void *dest, long size) const;
        virtual unsigned char * Dup(long size) const;
        virtual void ReadString(std::string& dest) const;
        virtual std::string ReadString() const;
        virtual void AppendString(std::string& dest) const;

        // Create a new BinaryPosition
        virtual SimpleBinaryPosition operator+
                                  (long additionalOffset) const;
        virtual SimpleBinaryPosition operator-
                                  (long additionalOffset) const;

        // Reposition the pointer
        virtual SimpleBinaryPosition& operator+=
                                         (long additionalOffset);
        virtual SimpleBinaryPosition& operator-=
                                         (long additionalOffset);
        virtual SimpleBinaryPosition& operator=(long offset);

        // atributes
        virtual const BinaryReader& Reader() const {return reader;}
        virtual long Offset() const {return offset;}
    private: 
        const BinaryReader& reader;
        long offset;
};

class SubReader: public BinaryReader {
public:
    SubReader(const BinaryPosition &p);
    typedef BinaryPosition PositionType;
    virtual void Read(long offset, void *dest, long size) const ;
    virtual void ReadString(long offset, std::string& dest)const;
    virtual SimpleBinaryPosition Begin() const =0;
    virtual SimpleBinaryPosition Pos(long offset) const =0;
private:
    SimpleBinaryPosition pos_;
};
#endif
