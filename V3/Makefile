# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

# Source files
SRCS = server.c \
       ../array.c \
       ../dictionary_loader.c \
       ../obj_string.c \
       ../object.c \
       ../wordpair.c \
       writer_thread.o \
       reader_thread.o

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = ../array.h ../dictionary_loader.h ../obj_string.h ../object.h ../wordpair.h \
          ../v2/writer_thread.h ../v2/reader_thread.h ../v2/shared.h ../v2/shared_array.h

# Output executable
TARGET = server

export DICTIONARY_FOLDER=../dictionary_folder

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile .c source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Pre-compiled writer_thread.o and reader_thread.o
writer_thread.o: ../v2/writer_thread.c ../v2/writer_thread.h
	$(CC) $(CFLAGS) -c $< -o $@

reader_thread.o: ../v2/reader_thread.c ../v2/reader_thread.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run the program with Valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)
