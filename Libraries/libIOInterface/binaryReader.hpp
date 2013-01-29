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
