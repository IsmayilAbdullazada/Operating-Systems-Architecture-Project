#include "../dictionary_loader.h" // For loading and managing dictionary files
#include "../array.h"            // Dynamic array implementation
#include "../wordpair.h"         // WordPair management
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DICTIONARY_FOLDER "../dictionary_folder" // Path to the dictionary folder

Array dictionary;  // Resizable array for WordPairs
Array knownFiles;  // Resizable array for file paths

// Signal handler for random WordPair translation
void handle_signal(int sig) {
    if (dictionary.size == 0) {
        printf("No word pairs available in the dictionary.\n");
        return;
    }

    int randomIndex = rand() % dictionary.size;
    WordPair *pair = (WordPair *)get_from_array(&dictionary, randomIndex);

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
        load_dictionary(folder, &dictionary, &knownFiles); // Use dictionary_loader function
        sleep(DICT_RELOAD_INTERVAL_SEC);  // Wait for the specified interval
    }

    return NULL;
}

int main() {
    // Initialize arrays for the dictionary and known files
    init_array(&dictionary, sizeof(WordPair), 10);
    init_array(&knownFiles, sizeof(char *), 10);

    printf("Process ID: %d\n", getpid());
    srand(time(NULL)); // Seed the random number generator

    // Create a thread for the dictionary loader
    pthread_t loader_thread;

    if (pthread_create(&loader_thread, NULL, loader_thread_func, (void *)DICTIONARY_FOLDER) != 0) {
        perror("Failed to create dictionary loader thread");
        return 1;
    }

    // Set up signal handlers
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    // Main loop: wait for signals
    while (1) {
        pause();
    }

    // Cleanup (won't reach here unless the program is terminated)
    pthread_cancel(loader_thread);
    pthread_join(loader_thread, NULL);

    // Free allocated memory
    free_known_files(&knownFiles);
    free_array(&knownFiles);

    for (size_t i = 0; i < dictionary.size; i++) {
        WordPair *pair = (WordPair *)get_from_array(&dictionary, i);
        free_wordpair(pair);
    }
    free_array(&dictionary);

    return 0;
}