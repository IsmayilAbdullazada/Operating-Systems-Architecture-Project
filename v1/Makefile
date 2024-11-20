# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread

# Source files
CLIENT_SRC = client.c
SERVER_SRC = server.c ../array.c ../dictionary_loader.c ../wordpair.c

# Header files
HEADERS = ../array.h ../dictionary_loader.h ../wordpair.h

# Targets
CLIENT_EXEC = client
SERVER_EXEC = server

# Default target
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Compile client executable
$(CLIENT_EXEC): $(CLIENT_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

# Compile server executable
$(SERVER_EXEC): $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC)

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)

# Phony targets
.PHONY: all clean
