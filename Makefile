BASEDIR=dns/
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=$(BASEDIR)main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(BASEDIR)dns

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

doxygen :
	@echo " DOXY Docs"
	@doxygen config/Doxyfile
