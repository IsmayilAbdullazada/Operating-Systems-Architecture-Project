#ifndef MESSAGE_H
#define MESSAGE_H
#define ENG_TO_FR_SIGNAL 1
#define FR_TO_ENG_SIGNAL 2
#define DELETE_MSG_SIGNAL 3 // Define a unique signal for delete messages
#define FILENAME_MAX_LENGTH 256
#ifndef WORD_MAX_LENGTH
#define WORD_MAX_LENGTH 50
#endif

typedef struct {
    long signal;            // Message type (used by msgrcv)
    char filename[FILENAME_MAX_LENGTH];     // Filename to delete words from
} Delete_msg;

typedef struct {
    long signal;            // Message type
    char word1[WORD_MAX_LENGTH];
    char word2[WORD_MAX_LENGTH];
    char filename[FILENAME_MAX_LENGTH];
} Add_msg;

#endif