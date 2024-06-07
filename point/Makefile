GCC = gcc
# GCC = g++
LIBS = -ljpeg -lm -largtable2
SOURCES = $(wildcard  *.c)
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -g -Wall

ifeq ($(OS),Windows_NT)
EXECUTABLE = point.exe
else
EXECUTABLE = point
endif


all: $(SOURCES) $(EXECUTABLE)



$(EXECUTABLE): $(OBJECTS) 
	$(GCC) $(OBJECTS) -o $@ $(LIBS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

