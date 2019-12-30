CC=g++
CFLAGS=-std=c++14 -m64 -msse4 -pthread -O3 -pedantic -Wall

perfTest: perfTest.cpp
	$(CC) $(CFLAGS) -o perfTest perfTest.cpp

clean:
	@rm -rf perfTest
