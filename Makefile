CC=gcc
CFLAGS=-Wall -g 
BINARY=icsh

all: icsh 

icsh: icsh.c parse_input.c built_in.c mode.c process.c signal_handler.c jobs_manager.c command_factory.c
	$(CC) -o $(BINARY) $(CFLAGS) icsh.c parse_input.c built_in.c mode.c process.c signal_handler.c jobs_manager.c command_factory.c

.PHONY: clean

clean:
	rm -f $(BINARY)
