BASEDIR=dns/
CC=g++
CFLAGS= -g -Werror=all -std=c++0x
SOURCES=$(BASEDIR)*.cpp
EXECUTABLE=$(BASEDIR)dns

all: 
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

doxygen :
	@echo " DOXY Docs"
	@doxygen config/Doxyfile
