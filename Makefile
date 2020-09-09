build: main.o
	clang++ -o bin/main -lsfml-window -lsfml-system -lsfml-graphics bin/main.o

analyze:
	pvs-studio-analyzer trace -- make build
	pvs-studio-analyzer analyze -o bin/report.log
	plog-converter -a GA:1,2 -t fullhtml bin/report.log -o errorreport/

test: tests.cpp sortlib
	clang++ -Wall -std=c++20 -o bin/test tests.cpp
	./bin/test

main.o: main.cpp sortlib
	clang++ -Wall -std=c++20 -c -o bin/main.o main.cpp

sortlib: sortstat.hpp abstractSort.hpp sorts.hpp