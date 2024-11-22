#include "../dictionary_loader.h"
#include "../array.h"
#include "../wordpair.h"
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DICTIONARY_FOLDER "../dictionary_folder"

Array* dictionary;
Array* knownFiles;
pthread_t loader_thread;
volatile sig_atomic_t terminate_flag = 0; // Flag for CTRL C termination

void cleanup() {
    printf("\nCleaning up resources...\n");

    pthread_cancel(loader_thread);
    pthread_join(loader_thread, NULL);

    Array_free(dictionary);
    Array_free(knownFiles);

    printf("Resources cleaned up. Exiting program.\n");
}

void handle_signal(int sig) {
    if (sig == SIGINT) {
        terminate_flag = 1;
        return;
    }

    if (dictionary->size == 0) {
        printf("No word pairs available in the dictionary.\n");
        return;
    }

    int randomIndex = rand() % dictionary->size;
    WordPair *pair = (WordPair *)Array_get(dictionary, randomIndex);

    if (sig == SIGUSR1) {
        printf("English: %s, French: %s\n", pair->english, pair->french);
    } else if (sig == SIGUSR2) {
        printf("French: %s, English: %s\n", pair->french, pair->english);
    }
}

void *loader_thread_func(void *arg) {
    const char *folder = (const char *)arg;
    while (1) {
        load_dictionary(folder, dictionary, knownFiles);
        sleep(DICT_RELOAD_INTERVAL_SEC);
        pthread_testcancel();
    }

    return NULL;
}

int main() {
    dictionary = Array_new(10);
    knownFiles = Array_new(10);

    printf("Process ID: %d\n", getpid());
    srand(time(NULL));

    if (pthread_create(&loader_thread, NULL, loader_thread_func, (void *)DICTIONARY_FOLDER) != 0) {
        perror("Failed to create dictionary loader thread");
        return 1;
    }

    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGINT, handle_signal); // Handle Ctrl+C (SIGINT) for cleanup

    // Main loop: wait for signals
    while (!terminate_flag) {
        pause();
    }

    cleanup();

    return 0;
}
