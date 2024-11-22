CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread

SRCS = main.c reader_thread.c writer_thread.c ../array.c ../obj_string.c ../dictionary_loader.c ../wordpair.c ../object.c

OBJS = $(SRCS:.c=.o)

HEADERS = reader_thread.h writer_thread.h shared.h message.h shared_array.h ../array.h ../obj_string.h ../dictionary_loader.h ../wordpair.h ../object.h

TARGET = main

export DICTIONARY_FOLDER=../dictionary_folder

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)