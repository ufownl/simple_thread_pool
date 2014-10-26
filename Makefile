CXX			= g++
LD			= g++
CXXFLAGS	= -std=c++11 -g -Wall -O3
LDFLAGS		=

all: example parallel_sort

example: example.o thread_pool.o
	$(LD) $^ $(LDFLAGS) -o $@

parallel_sort: parallel_sort.o thread_pool.o
	$(LD) $^ $(LDFLAGS) -o $@

example.o: example.cpp thread_pool.h
parallel_sort.o: parallel_sort.cpp thread_pool.h
thread_pool.o: thread_pool.cpp thread_pool.h

clean:
	$(RM) *.o
	$(RM) example
	$(RM) parallel_sort

.PHONY: all clean
