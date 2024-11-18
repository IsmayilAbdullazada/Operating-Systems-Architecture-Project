#include <dirent.h>
#include <errno.h>
#include "array.h"
#include <dirent.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep(), getpid(), pause()


#ifndef PATH_MAX
#define PATH_MAX 1012  // Maximum path length
#endif

#define DICT_RELOAD_INTERVAL_SEC 10  // Interval for reloading the dictionary in seconds

Array dictionary;  // Resizable array for WordPairs
Array knownFiles;  // Resizable array for file paths


// WordPair structure for storing English-French word pairs
typedef struct {
    char *english; // Dynamically allocated string for English word
    char *french;  // Dynamically allocated string for French word
} WordPair;

// Function to free WordPairs
void free_wordpair(Array *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        WordPair *pair = (WordPair *)get_from_array(arr, i);
        free(pair->english);
        free(pair->french);
    }
}

// Function to check if a file is already known
int is_known_file(const char *filename) {
    for (size_t i = 0; i < knownFiles.size; i++) {
        char *knownFile = *(char **)get_from_array(&knownFiles, i); // Use pointer dereference correctly
        if (strcmp(knownFile, filename) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to mark a file as known
void add_to_known_files(const char *filename) {
    char *filePath = strdup(filename); // Create a copy of the filename
    if (!filePath) {
        perror("Failed to allocate memory for filename");
        exit(EXIT_FAILURE);
    }
    add_to_array(&knownFiles, &filePath);
}

// Function to free KnownFiles
void free_known_files(Array *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        char *filePath = *(char **)get_from_array(arr, i);
        free(filePath);
    }
}

// Function to load the dictionary from a file
void load_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL) {
            WordPair pair;
            char *english = strtok(line, ";");
            char *french = strtok(NULL, "\n");

            if (english && french) {
                // Allocate and copy strings
                pair.english = strdup(english);
                pair.french = strdup(french);

                // Add to the dictionary
                add_to_array(&dictionary, &pair);
            }
        }
        fclose(file);
    } else {
        perror("Error opening file");
    }
}

// Function to load new dictionary files from the folder
void load_dictionary(const char *folder_path) {
    DIR *dir;
    struct dirent *ent;
    char filepath[PATH_MAX];

    if ((dir = opendir(folder_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

            snprintf(filepath, PATH_MAX, "%s/%s", folder_path, ent->d_name);
            if (!is_known_file(filepath)) {
                printf("Loading new file: %s\n", filepath);
                load_file(filepath);
                add_to_known_files(filepath);
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }
}



void handle_signal(int sig) {

    int randomIndex = rand() % dictionary.size;
    WordPair *pair = (WordPair *)get_from_array(&dictionary, randomIndex);
    if (sig == SIGUSR1) {
        printf("English: %s, French: %s\n", pair->english, pair->french);
    } else if (sig == SIGUSR2) {
        printf("French: %s, English: %s\n", pair->french, pair->english);
    }
}



// Thread function to periodically load the dictionary
void *dictionary_loader(void *arg) {
    const char *folder = (const char *)arg;
    while (1) {
        load_dictionary(folder);  // Load the dictionary
        sleep(DICT_RELOAD_INTERVAL_SEC);  // Wait for the specified interval
    }
    return NULL;  // Not used but required by pthread
}

int main() {
    init_array(&dictionary, sizeof(WordPair), 10); // Initialize dictionary
    init_array(&knownFiles, sizeof(char *), 10);   // Initialize known files array


    printf("Process ID: %d\n", getpid());
    srand(time(NULL));  // Seed the random number generator

    // Load the dictionary from the specified folder once initially
    const char *dictionary_folder = "dictionary_folder";
    load_dictionary(dictionary_folder);

    // Set up signal handlers
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    // Create a separate thread for periodic dictionary loading
    pthread_t loader_thread;
    if (pthread_create(&loader_thread, NULL, dictionary_loader, (void *)dictionary_folder) != 0) {
        perror("Failed to create dictionary loader thread");
        return 1;
    }

    // Main server loop to handle signals
    while (1) {
        pause();  // Wait for signals
    }

    return 0;
}
