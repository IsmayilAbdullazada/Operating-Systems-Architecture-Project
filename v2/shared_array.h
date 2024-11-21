#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H
#include <stdlib.h>

#define ARRAY_CAPACITY (10 * 1024 * 1024 / sizeof(WordPair_shared)) // Capacity based on 10 MB

#ifndef WORD_MAX_LENGTH
#define WORD_MAX_LENGTH 50
#endif

// Word pair structure
typedef struct {
    char english[WORD_MAX_LENGTH];
    char french[WORD_MAX_LENGTH];
    char filename[256];
} WordPair_shared;

// Shared array structure
typedef struct {
    size_t size;               // Current number of elements
    size_t capacity;           // Maximum capacity
    WordPair_shared array[];          // Flexible array for word pairs
} SharedArray;

#endif // SHARED_ARRAY_H