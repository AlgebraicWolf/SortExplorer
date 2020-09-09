build: main.o
	clang++ -o bin/main bin/main.o

main.o: main.cpp abstractSort.hpp sorts.hpp sortstat.hpp
	clang++ -Wall -std=c++20 -c -o bin/main.o main.cpp