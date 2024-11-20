#include "wordpair.h"
#include <string.h>
#include <stdio.h>

// Allocate a WordPair
WordPair create_wordpair(const char *english, const char *french, const char *sourceFile) {
  // printf("Creating WordPair: %s -> %s\n", english, french);  // Debugging line
    WordPair pair;
    pair.english = strdup(english);
    pair.french = strdup(french);
    pair.sourceFile = strdup(sourceFile); // Track the source file
    // Remove trailing newline from the French word
    size_t len = strlen(french);
    pair.french[len-1] = '\0';
    if (!pair.english || !pair.french || (sourceFile && !pair.sourceFile)) {
        perror("Failed to allocate memory for WordPair");
        exit(EXIT_FAILURE);
    }
    return pair;
}

// Function to free WordPairs
void free_wordpair(WordPair *pair) {
    free(pair->english);
    free(pair->french);
    free(pair->sourceFile);
    free(pair);
}