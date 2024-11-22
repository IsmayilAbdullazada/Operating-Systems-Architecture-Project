#include "../dictionary_loader.h"
#include "../objects/array.h"
#include "../objects/wordpair.h"
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

void *signal_handler_thread_func(void *arg) {
    int sig = *(int *)arg;
    free(arg);

    if (terminate_flag) {
        pthread_exit(NULL);
    }

    if (sig == SIGINT) {
        printf("\nSIGINT received, terminating program...\n");
        terminate_flag = 1;
        pthread_exit(NULL);
    }


    if (dictionary->size == 0) {
        printf("No word pairs available in the dictionary.\n");
        pthread_exit(NULL);
    }

    int randomIndex = rand() % dictionary->size;
    WordPair *pair = (WordPair *)Array_get(dictionary, randomIndex);

    if (sig == SIGUSR1) {
        printf("English: %s, French: %s\n", pair->english, pair->french);
    } else if (sig == SIGUSR2) {
        printf("French: %s, English: %s\n", pair->french, pair->english);
    }

    pthread_exit(NULL);
}

void signal_handler(int sig) {
    pthread_t signal_thread;
    int *arg = malloc(sizeof(int));
    if (arg == NULL) {
        perror("malloc");
        return;
    }

    *arg = sig;

    if (pthread_create(&signal_thread, NULL, signal_handler_thread_func, arg) != 0) {
        perror("pthread_create");
        free(arg);
        return;
    }

    pthread_detach(signal_thread);
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

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    // Main loop: wait for signals
    while (!terminate_flag) {
        pause();
    }

    cleanup();

    return 0;
}
