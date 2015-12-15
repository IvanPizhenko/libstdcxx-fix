.SUFFIXES:
.PHONY: all build clean rebuild test

TARGETS:=test1

CXX:=g++
CXXFLAGS:=-std=c++11 -Wall -Werror -Wpedantic -Wextra -Weffc++ -fmax-errors=3
INCLUDES:=-I../include

ifeq ("$(DEBUG)", "1")
CXXFLAGS+=-g3 -ggdb
endif

all: build

rebuild:
	echo Rebulding...
	$(MAKE) clean
	$(MAKE) build
	
clean:
	echo Cleaning up...
	-rm -f $(TARGETS)
	-rm -f *.o

build: $(TARGETS)
	echo Build finished.

test: $(TARGETS)
	echo Running test...
	./test1

test1: test1.cpp
	$(CXX) -o $@ $(INCLUDES) $(CXXFLAGS) $^
