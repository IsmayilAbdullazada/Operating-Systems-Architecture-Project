#ifndef WORDPAIR_H
#define WORDPAIR_H

#include <stdlib.h>
#include "array.h"

// WordPair structure for storing English-French word pairs
typedef struct {
    char *english; // Dynamically allocated string for English word
    char *french;  // Dynamically allocated string for French word
    char *sourceFile;  // Dynamically allocated string for source file path
} WordPair;

WordPair create_wordpair(const char *english, const char *french, const char *sourceFile);

void free_wordpair(WordPair *pair);

#endif