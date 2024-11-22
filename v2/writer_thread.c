#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dirent.h>
#include "../array.h"
#include "../wordpair.h"
#include "../obj_string.h"
#include "../dictionary_loader.h"
#include "message.h"
#include <unistd.h>
#include "shared.h"

void send_add_message(int msgid, const char *word1, const char *word2, const char *file, long signal) {
    Add_msg  message = {0};
    message.signal = signal;
    strncpy(message.word1, word1, WORD_MAX_LENGTH - 1);
    strncpy(message.word2, word2, WORD_MAX_LENGTH - 1);
    strncpy(message.filename, file, FILENAME_MAX_LENGTH);
    
    if (msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0) == -1) {
        perror("msgsnd");
    }
}

void send_remove_message(int msgid, const char *file, long signal) {
    Delete_msg message = {0};
    message.signal = signal;
    strncpy(message.filename, file, FILENAME_MAX_LENGTH);

    if (msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0) == -1) {
        perror("msgsnd");
    }
}

void process_added_file(int msgid, Array *dictionary, Array *knownFiles, const char *fileStr) {
    String *newFile = String_new(fileStr);
    Array_add(knownFiles, (Object *)newFile);

    Array *filePairs = Array_new(10);
    load_file(fileStr, filePairs);

    for (size_t i = 0; i < filePairs->size; i++) {
        WordPair *pair = (WordPair *)Array_get(filePairs, i);

        Array_add(dictionary, (Object *)pair);

        long signal = 0;
        if (strncmp(pair->sourceFile, "eng_to_fr", 9) == 0) {
            signal = ENG_TO_FR_SIGNAL;
        } else if (strncmp(pair->sourceFile, "fr_to_eng", 9) == 0) {
            signal = FR_TO_ENG_SIGNAL;
        } else {
            fprintf(stderr, "Invalid file name: %s\n", pair->sourceFile);
            continue;
        }

        send_add_message(msgid, pair->english, pair->french, pair->sourceFile, signal);
    }

    Array_free_shallow(filePairs);

}

void process_removed_file(int msgid, Array *dictionary, const char *filePath) {
    Array_filter(dictionary, cmpSourceFiles, filePath);
    send_remove_message(msgid, filePath, DELETE_MSG_SIGNAL);
}

void *writer_thread(void *arg) {
    int msgid = *(int *)arg;

    // Get folder path from environment variable
    char *folder_path = getenv("DICTIONARY_FOLDER");
    if (!folder_path) {
        fprintf(stderr, "Environment variable DICTIONARY_FOLDER is not set.\n");
        pthread_exit(NULL);
    }

    // Change working directory
    if (chdir(folder_path) != 0) {
        perror("chdir");
        printf("Failed to change directory to %s\n", folder_path);
        pthread_exit(NULL);
    }

    Array *dictionary = Array_new(10);
    Array *knownFiles = Array_new(10);
    while (!terminate_flag) {
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(folder_path)) != NULL) {
            
            // Detect removed files
            for (size_t i = 0; i < knownFiles->size; ) {
                Object *knownFileString  = Array_get(knownFiles, i);
                const char *knownFile = knownFileString->to_string(knownFileString);

                if (access(knownFile, F_OK) != 0) {
                    printf("File removed: %s\n", knownFile);
                    process_removed_file(msgid, dictionary, knownFile);

                    Array_remove_at(knownFiles, i);
                } else {
                    i++;
                }
            }

            // Detect added files
            while ((ent = readdir(dir)) != NULL) {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

                String *fileStr = String_new(ent->d_name);

                if (!Array_in(knownFiles, (Object *)fileStr)) {
                    printf("New file detected: %s\n", ent->d_name);

                     if (!(strncmp(ent->d_name, "eng_to_fr", 9) == 0 || strncmp(ent->d_name, "fr_to_eng", 9) == 0)) {
                        printf("Invalid file name: %s\n", ent->d_name);
                     } else {
                        process_added_file(msgid, dictionary, knownFiles, ent->d_name);
                     }

                }
                String_free((Object *)fileStr);
                
            }
            closedir(dir);


        } else {
            perror("Could not open directory");
            pthread_exit(NULL);
        }

        sleep(DICT_RELOAD_INTERVAL_SEC); // Periodic check for changes
    }

    Array_free(dictionary);
    Array_free(knownFiles);

    printf("Writer thread terminating...\n");
    pthread_exit(NULL);
}
