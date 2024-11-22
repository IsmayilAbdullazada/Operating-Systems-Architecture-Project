CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

SERVER_SRCS = server.c \
       ../objects/array.c \
       ../dictionary_loader.c \
       ../objects/obj_string.c \
       ../objects/object.c \
       ../objects/wordpair.c \
       writer_thread.o \
       reader_thread.o

CLIENT_SRCS = client.c

SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

HEADERS = ../objects/array.h ../objects/obj_string.h ../objects/object.h ../objects/wordpair.h \
          ../v2/writer_thread.h ../v2/reader_thread.h ../v2/shared.h ../v2/shared_array.h

SERVER_TARGET = server
CLIENT_TARGET = client

export DICTIONARY_FOLDER=../dictionary_folder

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

writer_thread.o: ../v2/writer_thread.c ../v2/writer_thread.h
	$(CC) $(CFLAGS) -c $< -o $@

reader_thread.o: ../v2/reader_thread.c ../v2/reader_thread.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET) $(CLIENT_TARGET)

run: $(SERVER_TARGET)
	./$(SERVER_TARGET)

run_client: $(CLIENT_TARGET)
	@echo "Please enter the server PID to send signals:"
	@read pid; ./$(CLIENT_TARGET) $$pid

valgrind: $(SERVER_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(SERVER_TARGET)

valgrind_client: $(CLIENT_TARGET)
	valgrind --suppressions=valgrind.supp --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(CLIENT_TARGET)