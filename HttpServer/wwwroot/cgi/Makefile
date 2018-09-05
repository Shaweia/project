.PHONY:all

all:netCal instruction

netCal:netCal.c
	gcc -o $@ $^

instruction:instruction.cc
	g++ -o $@ $^ --std=c++11


.PHONY:clean

clean:
	rm -rf instruction netCal
