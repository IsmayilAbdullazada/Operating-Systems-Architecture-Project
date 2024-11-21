#include "../dictionary_loader.h" // For loading and managing dictionary files
#include "../array.h"            // Dynamic array implementation
#include "../wordpair.h"         // WordPair management
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DICTIONARY_FOLDER "../dictionary_folder" // Path to the dictionary folder

Array* dictionary;  // Resizable array for WordPairs
Array* knownFiles;  // Resizable array for file paths
pthread_t loader_thread;
volatile sig_atomic_t terminate_flag = 0; // Flag for graceful termination

// Cleanup function
void cleanup() {
    printf("\nCleaning up resources...\n");

    // Cancel and join the loader thread
    pthread_cancel(loader_thread);
    pthread_join(loader_thread, NULL);

    // Free the dictionary and known files
    Array_free(dictionary);
    Array_free(knownFiles);

    printf("Resources cleaned up. Exiting program.\n");
}

// Signal handler for random WordPair translation and termination
void handle_signal(int sig) {
    if (sig == SIGINT) {
        terminate_flag = 1; // Set termination flag for cleanup
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

// Thread function for periodic dictionary loading
void *loader_thread_func(void *arg) {
    const char *folder = (const char *)arg;
    while (1) {
        load_dictionary(folder, dictionary, knownFiles); // Use dictionary_loader function
        sleep(DICT_RELOAD_INTERVAL_SEC);  // Wait for the specified interval
        pthread_testcancel(); // Allow thread cancellation at this point
    }

    return NULL;
}

int main() {
    dictionary = Array_new(10); // Initialize the dictionary array
    knownFiles = Array_new(10); // Initialize the known files array



    printf("Process ID: %d\n", getpid());
    srand(time(NULL)); // Seed the random number generator

    // Create a thread for the dictionary loader
    if (pthread_create(&loader_thread, NULL, loader_thread_func, (void *)DICTIONARY_FOLDER) != 0) {
        perror("Failed to create dictionary loader thread");
        return 1;
    }

    // Set up signal handlers
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGINT, handle_signal); // Handle Ctrl+C (SIGINT) for cleanup

    // Main loop: wait for signals
    while (!terminate_flag) {
        pause();
    }

    // Perform cleanup
    cleanup();

    return 0;
}