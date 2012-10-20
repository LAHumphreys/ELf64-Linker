INCLUDE_FLAGS="-I linux"
DEBUG_FLAGS="-ggdb"
archiveTest: archivetest.cpp elfReader.o elfParser.o elfHeader.o section.o flags.o stringTable.o symbol.o programHeader.o reloc.o binaryData.o binaryReader.o binaryWriter.o arc.o stdWriter.o
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -std=c++0x -o archiveTest archivetest.cpp  elfParser.o elfReader.o elfHeader.o section.o flags.o stringTable.o symbol.o programHeader.o reloc.o arc.o binaryData.o stdWriter.o binaryReader.o binaryWriter.o
elf2Link: elf2Link.cpp elfReader.o elfParser.o  elfHeader.o section.o flags.o stringTable.o symbol.o programHeader.o reloc.o binaryData.o binaryReader.o arc.o
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -std=c++0x -o elf2Link elf2Link.cpp elfParser.o elfReader.o elfHeader.o section.o flags.o stringTable.o symbol.o programHeader.o reloc.o arc.o binaryData.o binaryReader.o stdWriter.o
main: main.cpp elfReader.o elfParser.o elfHeader.o section.o flags.o stringTable.o symbol.o programHeader.o reloc.o binaryData.o stdWriter.o binaryReader.o binaryWriter.o arc.o 
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -std=c++0x -o main main.cpp  elfParser.o elfReader.o elfHeader.o section.o flags.o stringTable.o symbol.o programHeader.o reloc.o arc.o binaryData.o stdWriter.o binaryReader.o binaryWriter.o

reader.o: reader.cpp reader.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o reader.o reader.cpp

x86Parser.o: x86Parser.cpp x86Parser.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o x86Parser.o x86Parser.cpp

elfParser.o: elfParser.cpp elfParser.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o elfParser.o elfParser.cpp

elfHeader.o: elfHeader.cpp elfHeader.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o elfHeader.o elfHeader.cpp

section.o: section.cpp section.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o section.o section.cpp

stringTable.o: stringTable.h stringTable.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o stringTable.o stringTable.cpp

flags.o: flags.h flags.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o flags.o flags.cpp

elfReader.o: elfReader.h elfReader.cpp binaryReader.o
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o elfReader.o elfReader.cpp

symbol.o: symbol.cpp symbol.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o symbol.o symbol.cpp

programHeader.o: programHeader.cpp programHeader.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o programHeader.o programHeader.cpp

reloc.o: reloc.cpp reloc.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o reloc.o reloc.cpp

binaryData.o: binaryData.h binaryData.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o binaryData.o binaryData.cpp

binaryReader.o: binaryReader.cpp binaryReader.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o binaryReader.o binaryReader.cpp

binaryWriter.o: binaryWriter.cpp binaryWriter.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o binaryWriter.o binaryWriter.cpp

stdWriter.o: stdWriter.cpp stdWriter.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o stdWriter.o stdWriter.cpp

arc.o: arc.cpp arc.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o arc.o arc.cpp
  


clean: 
	rm *.o 
	[ -e main ] && rm main 
	[ -e elf2Link ] && rm elf2Link
	[ -e archiveTest ] && rm archiveTest
