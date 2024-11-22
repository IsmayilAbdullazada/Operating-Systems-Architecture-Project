#include "wordpair.h"
#include <string.h>
#include <stdio.h>
#include "object.h"
#include "obj_string.h"

const char *WordPair_to_string(Object *self) {
    WordPair *wp = (WordPair *)self;
    static char buffer[256];
    
    snprintf(buffer, sizeof(buffer), "{English: \"%s\", French: \"%s\", SourceFile: \"%s\"}",
             wp->english, wp->french, wp->sourceFile);
    return buffer;
}

// Initialize a WordPair
void WordPair_init(WordPair *self, const char *english, const char *french, const char *sourceFile) {
    Object_init((Object *)self);
    self->base.free = WordPair_free;
    self->base.to_string = WordPair_to_string;
    self->english = strdup(english);
    self->french = strdup(french);
    self->sourceFile = strdup(sourceFile);

    if (!self->english || !self->french || (sourceFile && !self->sourceFile)) {
        perror("Failed to allocate memory for WordPair");
        exit(EXIT_FAILURE);
    }
}

void WordPair_free(Object *self) {
    WordPair *wp = (WordPair *)self;
    free(wp->english);
    free(wp->french);
    free(wp->sourceFile);
    Object_free(self);
}

WordPair *WordPair_new(const char *english, const char *french, const char *sourceFile) {
    WordPair *wp = (WordPair *)malloc(sizeof(WordPair));
    if (!wp) {
        perror("Failed to allocate memory for WordPair");
        exit(EXIT_FAILURE);
    }
    WordPair_init(wp, english, french, sourceFile);
    return wp;
}


int cmpSourceFiles(const Object *a, const void *b) {
    const WordPair *pair = (const WordPair *)a;
    const char *sourceFile = (const char *)b;
    return strcmp(pair->sourceFile, sourceFile);
    
}