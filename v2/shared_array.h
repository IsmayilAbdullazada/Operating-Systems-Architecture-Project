#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H
#include <stdlib.h>

#define ARRAY_CAPACITY (10 * 1024 * 1024 / sizeof(WordPair_shared)) // 10 MB capacity

#ifndef WORD_MAX_LENGTH
#define WORD_MAX_LENGTH 50
#endif

typedef struct {
    char english[WORD_MAX_LENGTH];
    char french[WORD_MAX_LENGTH];
    char filename[256];
} WordPair_shared;

typedef struct {
    size_t size;
    size_t capacity;
    WordPair_shared array[];
} SharedArray;

#endif