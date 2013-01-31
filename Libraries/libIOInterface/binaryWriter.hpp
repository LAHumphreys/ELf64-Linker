// Reposition the pointer
template <typename number>
BinaryWriter BinaryWriter::operator+( number additionalOffset) const
{
    BinaryWriter p(this->file,additionalOffset + offset);
    return p;
}
//
// Reposition the pointer
template <typename number>
BinaryWriter BinaryWriter::operator-( number additionalOffset) const
{
    BinaryWriter p(this->file,offset - additionalOffset);
    return p;
}

template<class T>
BinaryWriter& BinaryWriter::Push(T& src) {
    Write(&src,sizeof(T));
    offset+=sizeof(T);
    return *this;
}

template<class T>
BinaryWriter& BinaryWriter::operator<<(const T& src) {
    return Push(src);
}
