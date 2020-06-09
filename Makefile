.PHONY: all clean

CXXFLAGS := -std=c++11

all:
	g++ $(CXXFLAGS) main.cpp -o test
	
