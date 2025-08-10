CXX=g++

loader.o: ../loader/loader.cpp
	$(CXX) -std=c++11 -c ../loader/loader.cpp

basic_capstone_linear: loader.o basic_capstone_linear.cpp
	$(CXX) -std=c++11 -o basic_capstone_linear basic_capstone_linear.cpp loader.o -lbfd -lcapstone

clean:
	rm -rf *.o basic_capstone_linear

.PHONY: clean
