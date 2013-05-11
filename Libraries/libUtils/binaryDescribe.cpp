#include "binaryDescribe.h"
#include "binaryReader.h"
#include <sstream>
#include <iomanip>
#include <string>

std::string BinaryDescribe::Describe(const BinaryReader& src, long len) {
    stringstream output("");
    BinaryReader reader(src);
    for (long index=0; index < len; index+=16) {
        // Reset this line
        stringstream line("");
        stringstream binstr("");
        stringstream asciistr("");

        line << hex << setw(8) << setfill('0') << index;
        line << " ";

        asciistr << "|";
        for ( int i=0; i<16; i++) {
            if (i%8 == 0)
                binstr << " ";
            // Get each character
            unsigned char c = reader.Get();
            reader+=1;

            // Write the binary #
            unsigned short n = c - 0;
            binstr << hex << setw(2) << setfill('0') << n;
            binstr << " ";

            // Write the ascii char
            asciistr << GetAscii(c);
        }
        // finish off the ascii
        asciistr << "|";

        // combine the strings
        line << binstr.str() << " " << asciistr.str();
        output << line.str() << endl;
    }
    return output.str();
}

char BinaryDescribe::GetAscii(char in) {
    if ( in < ' ' || in > '~' ) {
        return '.';
    } else {
        return in;
    }
}
