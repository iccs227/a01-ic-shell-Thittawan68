CC=gcc
CFLAGS=-Wall -g 
BINARY=icsh

all: icsh 

icsh: icsh.c parse_input.c built_in.c mode.c process.c
	$(CC) -o $(BINARY) $(CFLAGS) icsh.c parse_input.c built_in.c mode.c process.c

.PHONY: clean

clean:
	rm -f $(BINARY)
