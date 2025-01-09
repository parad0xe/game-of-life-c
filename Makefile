BASEDIR= ./src/

CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=`sdl2-config --cflags --libs` -lm
I=
EXECUTABLE=main

SOURCES = $(BASEDIR)main.c \
	$(BASEDIR)game.c \
	$(BASEDIR)environment.c

HEADERS = $(BASEDIR)constants.h

OBJECTS = $(SOURCES:.c=.o)

.SILENT:

.PHONY: all
all: $(EXECUTABLE)

.PHONY: main
main : $(OBJECTS)
	$(CC) $(I) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	@echo "Compiling ($(EXECUTABLE)).. done"

.PHONY: clean
clean:
	rm -f $(BASEDIR)*.o $(EXECUTABLE)
	@echo "Cleaning.. done"

%.o : %.c $(HEADERS)
	$(CC) $(I) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
	@echo "Compiling ($<).. ok"
	
