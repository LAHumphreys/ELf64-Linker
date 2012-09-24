#!/usr/bin/python
from linker import *
from buildObjects import *
import sys

inputFiles = []
for file in sys.argv[1:]:
    inputFiles.append(GetParser(file))

output = ObjectFile()
for f in inputFiles:
    output.LinkInFile(f)
output.Allocate()
print output.ObjectFile()
