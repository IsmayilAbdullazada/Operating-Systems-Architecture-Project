#ifndef WORDPAIR_H
#define WORDPAIR_H
#include "object.h"
#include <stdlib.h>
#include "array.h"

// WordPair structure for storing English-French word pairs
typedef struct {
    Object base;    // Base Object
    char *english; // Dynamically allocated string for English word
    char *french;  // Dynamically allocated string for French word
    char *sourceFile;  // Dynamically allocated string for source file path
} WordPair;

void WordPair_init(WordPair *self, const char *english, const char *french, const char *sourceFile);
void WordPair_free(Object *self);
WordPair *WordPair_new(const char *english, const char *french, const char *sourceFile);
const char *WordPair_to_string(Object *self);

int cmpSourceFiles(const Object *a, const void *b);

#endif