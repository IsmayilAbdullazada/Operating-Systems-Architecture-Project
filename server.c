#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h> // For directory operations


#define DICT_RELOAD_INTERVAL_SEC 3

#define MAX_WORDS 1000
#define MAX_FILENAME_LENGTH 512
#define MAX_FILES 100 // Maximum number of files to track

typedef struct {
    char english[MAX_WORDS];
    char french[MAX_WORDS];
} WordPair;

WordPair dictionary[MAX_WORDS];

int wordCount = 0;
char knownFiles[MAX_FILES][MAX_FILENAME_LENGTH];
int knownFileCount = 0;

// Function to check if a file is already known
int is_known_file(const char *filename) {
    for (int i = 0; i < knownFileCount; i++) {
        if (strcmp(knownFiles[i], filename) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to mark a file as known
void add_to_known_files(const char *filename) {
    if (knownFileCount < MAX_FILES) {
        strncpy(knownFiles[knownFileCount], filename, MAX_FILENAME_LENGTH - 1);
        knownFiles[knownFileCount][MAX_FILENAME_LENGTH - 1] = '\0';
        knownFileCount++;
    }
}

// Function to load the dictionary from a file
void load_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file != NULL) {
        char line[101];
        while (fgets(line, sizeof(line), file) != NULL) {
            char *token = strtok(line, ";");
            if (token != NULL) {
                strcpy(dictionary[wordCount].english, token);
                token = strtok(NULL, ";");
                if (token != NULL) {
                    strcpy(dictionary[wordCount].french, token);
                    // Remove trailing newline from French word:
                    size_t len = strlen(dictionary[wordCount].french);
                    if (len > 0 && dictionary[wordCount].french[len - 1] == '\n') {
                        dictionary[wordCount].french[len - 1] = '\0';
                    }
                    wordCount++;
                }
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
    char filepath[MAX_FILENAME_LENGTH];

    if ((dir = opendir(folder_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue; // Skip . and ..

            snprintf(filepath, MAX_FILENAME_LENGTH, "%s/%s", folder_path, ent->d_name);
            if (!is_known_file(filepath)) {
                printf("Loading new file: %s\n", filepath);
                load_file(filepath);
                add_to_known_files(filepath); // Mark the file as known
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }
}

void handle_signal(int sig) {
    if (wordCount == 0) return; // Nothing to translate

    int randomIndex = rand() % wordCount;
    if (sig == SIGUSR1) {
        printf("English: %s, French: %s\n", dictionary[randomIndex].english, dictionary[randomIndex].french);
    } else if (sig == SIGUSR2) {
        printf("French: %s, English: %s\n", dictionary[randomIndex].french, dictionary[randomIndex].english);
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
