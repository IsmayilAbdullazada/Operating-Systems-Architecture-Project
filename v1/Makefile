CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread

CLIENT_SRC = client.c
SERVER_SRC = server.c ../array.c ../dictionary_loader.c ../wordpair.c ../object.c ../obj_string.c

HEADERS = ../array.h ../dictionary_loader.h ../wordpair.h  ../object.h ../obj_string.h

CLIENT_EXEC = client
SERVER_EXEC = server

all: $(CLIENT_EXEC) $(SERVER_EXEC)

$(CLIENT_EXEC): $(CLIENT_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

$(SERVER_EXEC): $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC) 

clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)

.PHONY: all clean