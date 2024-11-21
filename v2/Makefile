# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread

# Source files for the server
SERVER_SRC = main.c ../array.c ../dictionary_loader.c ../wordpair.c shared_memory.c message_queue.c

# Header files
HEADERS = ../array.h ../dictionary_loader.h ../wordpair.h shared_memory.h message_queue.h message.h

# Target executable
SERVER_EXEC = server

# Default target
all: $(SERVER_EXEC)

# Compile server executable
$(SERVER_EXEC): $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC)

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -f $(SERVER_EXEC) *.o

# Phony targets
.PHONY: all clean
