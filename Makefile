CXX			= g++
LD			= g++
CXXFLAGS	= -std=c++11 -g -Wall -O3
LDFLAGS		=

all: example parallel_sort param_passing

example: example.o thread_pool.o
	$(LD) $^ $(LDFLAGS) -o $@

parallel_sort: parallel_sort.o thread_pool.o
	$(LD) $^ $(LDFLAGS) -o $@

param_passing: param_passing.o thread_pool.o
	$(LD) $^ $(LDFLAGS) -o $@

example.o: example.cpp thread_pool.h
parallel_sort.o: parallel_sort.cpp thread_pool.h
param_passing.o: param_passing.cpp thread_pool.h
thread_pool.o: thread_pool.cpp thread_pool.h

clean:
	$(RM) *.o
	$(RM) example
	$(RM) parallel_sort
	$(RM) param_passing

.PHONY: all clean
