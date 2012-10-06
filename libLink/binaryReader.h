

// Interface for a common base class for binary readers, see
// ElfReader for an example implementation
class BinaryReader {
    virtual void Read(long offset, void *dest, long size) =0;
    virtual void ReadString(long offset, string& dest) =0;

    // We want to return these by reference to avoid a slice. We
    // don't really want a pointer since it puts an unreasonable
    // demand on the caller to clean up after us. 
    
    // Returing a reference allows the reader object to manage a
    // single position object which it can clean up. We demand it
    // to be const to protect this single object, and the caller
    // can use the operators to construct a new variable position
    // if required.
    virtual const BinaryPosition& Begin()=0;
    virtual const BinaryPosition& Pos(long offset)=0;
}

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
        virtual void Read(void *dest, long size)=0 const;
        virtual void * Dup(long size)=0 const;
        virtual void ReadString(string& dest)=0 const;
        virtual string ReadString()=0 const;

        // Create a new BinaryPosition

        virtual BinaryPosition operator+(long &additionalOffset)
                                                         =0 const;
        virtual BinaryPosition operator-(long &additionalOffset)
                                                         =0 const;

        // Reposition the pointer
        virtual BinaryPosition& operator+=(long &additionalOffset)=0;
        virtual BinaryPosition& operator-=(long &additionalOffset)=0;
        virtual BinaryPosition& operator=(long &offset)=0;

        // atributes
        virtual BinaryReader& Reader()=0 const;
        virtual long Offset()=0 const;
}

// Since all of the demands of BinaryPosition can be met by the
// interface required of BinaryReader, create a simple
// implementation.
class SimpleBinaryPosition: public BinaryPosition {
    public:
        SimpleBinaryPosition(BinaryReader &r, long offset);

        // Access data
        virtual void Read(void *dest, long size)=0 const;
        virtual void * Dup(long size)=0 const;
        virtual void ReadString(string& dest)=0 const;
        virtual string ReadString()=0 const;

        // Create a new BinaryPosition
        virtual BinaryPosition operator+(long &additionalOffset)
                                                             const;
        virtual BinaryPosition operator-(long &additionalOffset)
                                                             const;

        // Reposition the pointer
        virtual BinaryPosition operator+=(long &additionalOffset)=0;
        virtual BinaryPosition operator-=(long &additionalOffset)=0;
        virtual BinaryPosition operator=(long &offset)=0;

        // atributes
        virtual BinaryReader& Reader()=0 const;
        virtual long Offset()=0 const;
    private: 
        long offset;
        BinaryReader& reader;
}
