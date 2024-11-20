#include <stdio.h>
#include <pthread.h>
#include "../array.h"
#include "message.h"
#include "message_queue.h"
#include "shared_memory.h"

// Mutex for synchronizing access to the dictionary
pthread_mutex_t dictionary_mutex = PTHREAD_MUTEX_INITIALIZER;

void *translation_reader(void *arg) {
    Array *dictionary = (Array *)arg;

    while (1) {
        Message msg;
        
        // Receive a message from the queue
        if (receive_message(&msg) == -1) {
            perror("Failed to receive message");
            break;
        }

        // Log the received message
        printf("Reader: Received message: '%s' -> '%s' (Direction: %s)\n", 
               msg.pair.english, msg.pair.french, 
               (msg.msg_type == 1 ? "English-to-French" : "French-to-English"));

        // Lock the dictionary before modifying it
        pthread_mutex_lock(&dictionary_mutex);

        // Check if the dictionary has space for more word pairs
        if (dictionary->size == dictionary->capacity) {
            printf("Reader: Dictionary full. Unable to add new word pair.\n");
        } else {
            // If it's French-to-English, swap the pair
            if (msg.msg_type == 2) {
                // Swap English and French
                char *temp = msg.pair.english;
                msg.pair.english = msg.pair.french;
                msg.pair.french = temp;
            }

            // Add the word pair received in the message to the dictionary using the array functions
            add_to_array(dictionary, &msg.pair);
            printf("Reader: Added pair to dictionary: '%s' -> '%s'\n", 
                   msg.pair.english, msg.pair.french);
        }
        
        // Unlock the dictionary after modification
        pthread_mutex_unlock(&dictionary_mutex);

        // Sleep briefly to simulate processing time and give other tasks a chance
        sleep(1); // Adjust as needed
    }

    return NULL;
}
