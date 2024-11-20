#include <stdio.h>
#include "dictionary.h"
#include "message_queue.h"
#include "shared_memory.h"

void *translation_reader(void *arg) {
    Array *dictionary = (Array *)arg;

    while (1) {
        Message msg;
        if (receive_message(&msg) == -1) {
            perror("Failed to receive message");
            break;
        }

        pthread_mutex_lock(&dictionary_mutex);

        if (dictionary->size == dictionary->capacity) {
            printf("Dictionary full. Unable to add new word pair.\n");
        } else {
            add_to_array(dictionary, &msg.pair);
        }

        pthread_mutex_unlock(&dictionary_mutex);
    }

    return NULL;
}
