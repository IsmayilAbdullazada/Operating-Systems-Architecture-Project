#include "wordpair.h"
#include <string.h>
#include <stdio.h>
#include "object.h"
#include "obj_string.h"


// WordPair-specific to_string method
const char *WordPair_to_string(Object *self) {
    WordPair *wp = (WordPair *)self; // Cast to derived type
    static char buffer[256];
    
    snprintf(buffer, sizeof(buffer), "{English: \"%s\", French: \"%s\", SourceFile: \"%s\"}",
             wp->english, wp->french, wp->sourceFile);
    return buffer;
}

// Initialize a WordPair
void WordPair_init(WordPair *self, const char *english, const char *french, const char *sourceFile) {
    Object_init((Object *)self);           // Initialize the base object
    self->base.free = WordPair_free;       // Override the free method
    self->base.to_string = WordPair_to_string; // Override the to_string method
    self->english = strdup(english);      // Copy strings
    self->french = strdup(french);
    self->sourceFile = strdup(sourceFile);

    if (!self->english || !self->french || (sourceFile && !self->sourceFile)) {
        perror("Failed to allocate memory for WordPair");
        exit(EXIT_FAILURE);
    }
}

// WordPair-specific free method
void WordPair_free(Object *self) {
    WordPair *wp = (WordPair *)self; // Cast to derived type
    free(wp->english);
    free(wp->french);
    free(wp->sourceFile);
    Object_free(self); // Call base free method
}


// WordPair-specific new method
WordPair *WordPair_new(const char *english, const char *french, const char *sourceFile) {
    WordPair *wp = (WordPair *)malloc(sizeof(WordPair));
    if (!wp) {
        perror("Failed to allocate memory for WordPair");
        exit(EXIT_FAILURE);
    }
    WordPair_init(wp, english, french, sourceFile); // Initialize the object
    return wp;
}


int cmpSourceFiles(const Object *a, const void *b) {
    const WordPair *pair = (const WordPair *)a;
    const String *sourceFile = (const String *)b;
    return strcmp(pair->sourceFile, sourceFile->data);
    
}