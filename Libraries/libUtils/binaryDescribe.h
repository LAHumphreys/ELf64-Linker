#ifndef BINARY_DESCRIBE_H
#define BINARY_DESCRIBE_H
#include <string>
class BinaryReader;

class BinaryDescribe {
public:
    static std::string Describe(const BinaryReader& src, long len);
    static char GetAscii(char in);
};
#endif
