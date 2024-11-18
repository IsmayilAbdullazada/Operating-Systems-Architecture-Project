# Compiler and flags
CC = gcc
CFLAGS = 

# Targets
CLIENT_EXEC = client
SERVER_EXEC = server

# Default target
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Compile client executable
$(CLIENT_EXEC): client.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile server executable
$(SERVER_EXEC): server.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)

# Phony targets
.PHONY: all clean
