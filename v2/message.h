#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>
#include "../wordpair.h"

#define MSG_QUEUE_KEY 5678

// Structure for a message to be sent over the message queue
typedef struct {
    long msg_type;   // Message type (1 for English-to-French, 2 for French-to-English)
    WordPair pair;   // WordPair structure
} Message;

#endif
