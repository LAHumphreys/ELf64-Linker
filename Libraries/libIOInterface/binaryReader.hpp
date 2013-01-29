template<class T>
BinaryReader& BinaryReader::operator>>(T& dest) {
    return Pull(dest);
}

template<class T>
BinaryReader& BinaryReader::Pull(T& dest) {
    Read(&dest,sizeof(T));
    offset+=sizeof(T);
    return *this;
}

// Reposition the pointer
template <typename number>
BinaryReader BinaryReader::operator+( number additionalOffset) const
{
    BinaryReader p(this->file,additionalOffset + offset);
    return p;
}
//
// Reposition the pointer
template <typename number>
BinaryReader BinaryReader::operator-( number additionalOffset) const
{
    BinaryReader p(this->file,offset - additionalOffset);
    return p;
}
