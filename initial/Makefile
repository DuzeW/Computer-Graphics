CC = gcc
SOURCES = $(wildcard  *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = initials.png
CFLAGS = -g -Wall

ifeq ($(OS),Windows_NT)
LIBS = -l:libpng.a -l:libz.a -lm
EXECUTABLE = writepng.exe
else
LIBS = -lpng -lm
EXECUTABLE = writepng
endif


all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LIBS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS) $(TARGET)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(TARGET): run

test: run
	display $(TARGET)
