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
