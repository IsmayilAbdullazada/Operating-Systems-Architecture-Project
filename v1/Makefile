CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread

CLIENT_SRC = client.c
SERVER_SRC = server.c ../objects/array.c ../dictionary_loader.c ../objects/wordpair.c ../objects/object.c ../objects/obj_string.c

HEADERS = ../objects/array.h ../dictionary_loader.h ../objects/wordpair.h ../objects/object.h ../objects/obj_string.h

CLIENT_EXEC = client
SERVER_EXEC = server

all: $(CLIENT_EXEC) $(SERVER_EXEC)

$(CLIENT_EXEC): $(CLIENT_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

$(SERVER_EXEC): $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC)

clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)

valgrind-client: $(CLIENT_EXEC)
	valgrind --leak-check=full --track-origins=yes ./$(CLIENT_EXEC)

valgrind-server: $(SERVER_EXEC)
	valgrind --leak-check=full --track-origins=yes ./$(SERVER_EXEC)

.PHONY: all clean valgrind-client valgrind-server
