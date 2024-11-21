# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread

# Source files
SRCS = main.c reader_thread.c writer_thread.c ../array.c ../obj_string.c ../dictionary_loader.c ../wordpair.c ../object.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = reader_thread.h writer_thread.h shared.h message.h shared_array.h ../array.h ../obj_string.h ../dictionary_loader.h ../wordpair.h ../object.h

# Output executable
TARGET = main


export DICTIONARY_FOLDER=../dictionary_folder

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program (optional)
run: $(TARGET)
	./$(TARGET)


# Run the program with valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)