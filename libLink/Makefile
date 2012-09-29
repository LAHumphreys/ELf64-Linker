INCLUDE_FLAGS="-I linux"
main: main.cpp reader.o Makefile elfHeader.o
	g++ $(INCLUDE_FLAGS) -std=c++0x -o main main.cpp reader.o elfHeader.o

reader.o: reader.cpp reader.h
	g++ $(INCLUDE_FLAGS) -c -std=c++0x -o reader.o reader.cpp

elfHeader.o: elfHeader.cpp elfHeader.h
	g++ $(INCLUDE_FLAGS) -c -std=c++0x -o elfHeader.o elfHeader.cpp
