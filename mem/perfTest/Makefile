CC=g++
INCLUDE_DIR=-I$(VULKAN_SDK)/include
CFLAGS=-std=c++14 -m64 -msse4 -O3 -pedantic -Wall $(INCLUDE_DIR)
LDFLAGS=-pthread

perfTest: perfTest.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o perfTest perfTest.cpp

clean:
	@rm -rf perfTest
