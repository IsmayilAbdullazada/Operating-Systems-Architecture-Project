#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>

// Structure for a WordPair
typedef struct {
    char *english;  // Pointer to dynamically allocated English word
    char *french;   // Pointer to dynamically allocated French word
} WordPair;

// Structure for a message to be sent over the message queue
typedef struct {
    long msg_type;   // Message type (1 for English-to-French, 2 for French-to-English)
    WordPair pair;   // WordPair structure
} Message;

#endif
