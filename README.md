Given the messy nature of dealing with "real" object files it has been necessary to expand the format definition from the book. Changes are listed here:

Symbols:
Some convention for empty symbol names is required. "__blank__" has been chosen

The symbol definition has an extra flag: "scope" tacked on to the end:
     G: Global
     L: Local 

In the books definition a symbol has two types, D and U, but the other types returned by readelf seem like they could be useful. Here is the current map from readelf to LINK (note: "D" has been abandoned, and is instead implied by a lack of "U"):
     U: NOTYPE
     F: FILE 
     S: SECTION
     O: OBJECT
     P: FUNC (mnemonic: PROC)
     


Segment Headers:
An alignment value (int) has been tacked on to the end of the segment header. Value is in bytes

There are now a whole bunch of extra flags. Here is the mapping from objdump to LINK:
     P: CONTENTS
     R: READABLE
     W: WRITEABLE (objdump has a read-only flag, the absensce of which is treated as a W flag)
     M: RELOC
     D: DATA
     C: CODE
