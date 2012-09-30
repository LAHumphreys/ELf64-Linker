INCLUDE_FLAGS="-I linux"
DEBUG_FLAGS="-ggdb"
elf2Link: elf2Link.cpp elfReader.o elfParser.o elfHeader.o section.o flags.o stringTable.o
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -std=c++0x -o elf2Link elf2Link.cpp elfParser.o elfReader.o elfHeader.o section.o flags.o stringTable.o

main: main.cpp reader.o Makefile elfHeader.o x86Parser.o section.o stringTable.o flags.o elf2LINK
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -std=c++0x -o main main.cpp x86Parser.o reader.o elfHeader.o section.o stringTable.o flags.o

reader.o: reader.cpp reader.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o reader.o reader.cpp

x86Parser.o: x86Parser.cpp x86Parser.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o x86Parser.o x86Parser.cpp

elfHeader.o: elfHeader.cpp elfHeader.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o elfHeader.o elfHeader.cpp

section.o: section.cpp section.h
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o section.o section.cpp

stringTable.o: stringTable.h stringTable.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o stringTable.o stringTable.cpp

flags.o: flags.h flags.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o flags.o flags.cpp

elfReader.o: elfReader.h elfReader.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o elfReader.o elfReader.cpp

elfParser.o: elfParser.h elfParser.cpp
	g++ $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c -std=c++0x -o elfParser.o elfParser.cpp

clean: 
	rm *.o main elf2Link
