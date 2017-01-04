BASEDIR=dns/
CC=g++
CFLAGS= -g -Werror=all -std=c++0x
UTILS=$(BASEDIR)parser.cpp $(BASEDIR)exceptions.cpp
MAIN=$(BASEDIR)main.cpp
EXECUTABLE=$(BASEDIR)dns

TESTS=$(BASEDIR)unittests/*.cpp
FRAMEWORK=$(BASEDIR)unittests/*.hpp
UNITTESTS_EXECUTABLE=$(BASEDIR)tests

all: 
	$(CC) $(CFLAGS) $(UTILS) $(MAIN) -o $(EXECUTABLE)

tests:
	$(CC) $(CFLAGS) $(UTILS) $(FRAMEWORK) $(TESTS) -o $(UNITTESTS_EXECUTABLE)

install-deps:
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install libc6:i386 libstdc++6:i386 g++

doxygen :
	@echo " DOXY Docs"
	@doxygen config/Doxyfile
