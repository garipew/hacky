CC=g++
CFLAGS=-lbfd

demo: loader.o loader_demo.cpp
	$(CC) -o demo loader.o loader_demo.cpp $(CFLAGS) 

loader.o: loader.cpp loader.hpp
	$(CC) -c loader.cpp $(CFLAGS) 

clean:
	rm -rf demo *.o

.PHONY: clean
