.PHONY: all clean

CXXFLAGS := -std=c++11 -g -O0 -I./ -Wformat=0

all:
	g++ $(CXXFLAGS) main.cpp -o test

