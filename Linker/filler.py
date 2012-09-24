#!/usr/bin/python

import sys

count = 2 * int(sys.argv[1],16)
template = sys.argv[2]
out = ""
for i in range(count):
   out += template[i%len(template)]
print out
