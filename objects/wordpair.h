#ifndef WORDPAIR_H
#define WORDPAIR_H
#include "object.h"
#include <stdlib.h>
#include "array.h"

typedef struct {
    Object base;
    char *english;
    char *french;
    char *sourceFile;
} WordPair;

void WordPair_init(WordPair *self, const char *english, const char *french, const char *sourceFile);
void WordPair_free(Object *self);
WordPair *WordPair_new(const char *english, const char *french, const char *sourceFile);
const char *WordPair_to_string(Object *self);

int cmpSourceFiles(const Object *a, const void *b);

#endif