#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "message.h"
#include "shared.h"
#include "shared_array.h"
#include "reader_thread.h"
#include <pthread.h>

void *reader_thread(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int msgid = args->msgid;
    int shmid = args->shmid;

    SharedArray *shared_array = (SharedArray *)shmat(shmid, NULL, 0);
    if (shared_array == (SharedArray *)-1) {
        perror("shmat");
        pthread_exit(NULL);
    }

    shared_array->size = 0;
    shared_array->capacity = ARRAY_CAPACITY;

    printf("Reader thread started. Waiting for messages...\n");

    while (!terminate_flag) {
        // Use a buffer to handle messages of different types
        char message_buffer[sizeof(Delete_msg) > sizeof(Add_msg) ? sizeof(Delete_msg) : sizeof(Add_msg)];

        if (msgrcv(msgid, &message_buffer, sizeof(message_buffer) - sizeof(long), 0, 0) == -1) {
            if (terminate_flag)
                break;

            perror("msgrcv");
            continue;
        }

        // Cast to base structure to determine the signal type
        long signal = ((Add_msg *)message_buffer)->signal;

        if (signal == ENG_TO_FR_SIGNAL || signal == FR_TO_ENG_SIGNAL) {
            // Handle word addition messages
            Add_msg *add_msg = (Add_msg *)message_buffer;

            if (shared_array->size < shared_array->capacity) {
                WordPair_shared *pair = &shared_array->array[shared_array->size];

                if (signal == ENG_TO_FR_SIGNAL) {
                    strncpy(pair->english, add_msg->word1, WORD_MAX_LENGTH - 1);
                    strncpy(pair->french, add_msg->word2, WORD_MAX_LENGTH - 1);
                } else if (signal == FR_TO_ENG_SIGNAL) {
                    strncpy(pair->french, add_msg->word1, WORD_MAX_LENGTH - 1);
                    strncpy(pair->english, add_msg->word2, WORD_MAX_LENGTH - 1);
                }

                pair->english[WORD_MAX_LENGTH - 1] = '\0';
                pair->french[WORD_MAX_LENGTH - 1] = '\0';
                strncpy(pair->filename, add_msg->filename, 255);
                pair->filename[255] = '\0';

                shared_array->size++;
                printf("Added word pair: English: %s, French: %s, File: %s\n", pair->english, pair->french, pair->filename);
            } else {
                printf("Shared array is full. Cannot add more word pairs.\n");
            }

        } else if (signal == DELETE_MSG_SIGNAL) {
            // Handle delete messages
            Delete_msg *delete_msg = (Delete_msg *)message_buffer;
            printf("Delete request received for file: %s\n", delete_msg->filename);

            // Pair deletion from the shared array
            for (size_t i = 0; i < shared_array->size; ) {
                WordPair_shared *pair = &shared_array->array[i];
                if (strcmp(pair->filename, delete_msg->filename) == 0) {
                    printf("Deleting word pair: English: %s, French: %s\n", pair->english, pair->french);

                    memmove(pair, pair + 1, (shared_array->size - i - 1) * sizeof(WordPair_shared));
                    shared_array->size--;
                } else {
                    i++;
                }
            }

        } else {
            printf("Unknown signal type: %ld\n", signal);
        }
    }

    if (shmdt(shared_array) == -1) {
        perror("shmdt");
    }

    printf("Reader thread terminating...\n");
    return NULL;
}
