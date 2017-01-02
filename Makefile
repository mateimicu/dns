BASEDIR=dns/
CC=g++
# CFLAGS=-c -Wall
CFLAGS=-c -Werror=all -std=c++0x
SOURCES=$(BASEDIR)main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(BASEDIR)dns

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

doxygen :
	@echo " DOXY Docs"
	@doxygen config/Doxyfile
