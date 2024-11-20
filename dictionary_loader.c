#include "dictionary_loader.h"
// #include "wordpair.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // For `access()`

void load_file(const char *filepath, Array *dictionary) {
    FILE *file = fopen(filepath, "r");
    if (file != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL) {
            char *english = strtok(line, ";");
            char *french = strtok(NULL, "\n");

            if (english && french) {
                WordPair pair = create_wordpair(english, french, filepath);
                add_to_array(dictionary, &pair);
            }
        }
        fclose(file);
    } else {
        perror("Error opening file");
    }
}

// Function to load new dictionary files from the folder
void load_dictionary(const char *folder_path, Array *dictionary, Array *knownFiles) {
    DIR *dir;
    struct dirent *ent;
    char filepath[PATH_MAX];

    if ((dir = opendir(folder_path)) != NULL) {
        // Load new files from the folder
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            // printf("%s\n", ent->d_name);
            snprintf(filepath, PATH_MAX, "%s/%s", folder_path, ent->d_name);

            if (!is_known_file(filepath, knownFiles)) {
                printf("Loading new file: %s\n", filepath);
                load_file(filepath, dictionary);
                add_to_known_files(filepath, knownFiles);
            }
        }
        closedir(dir);

        // Check for removed files
        for (size_t i = 0; i < knownFiles->size; ) {
            char *knownFile = *(char **)get_from_array(knownFiles, i);

            // If the file no longer exists, remove it
            if (access(knownFile, F_OK) != 0) {
                printf("File removed: %s\n", knownFile);

                // Remove words from the dictionary associated with the file
                remove_words_from_dictionary(knownFile, dictionary);

                // Remove from knownFiles
                remove_from_known_files(i, knownFiles);
            } else {
                i++; // Only increment if not removed
            }
        }
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }
}

int is_known_file(const char *filename, Array *knownFiles) {
    for (size_t i = 0; i < knownFiles->size; i++) {
        char *knownFile = *(char **)get_from_array(knownFiles, i); // Use pointer dereference correctly
        if (strcmp(knownFile, filename) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_to_known_files(const char *filename, Array *knownFiles) {
    char *filePath = strdup(filename);
    if (!filePath) {
        perror("Failed to allocate memory for filename");
        exit(EXIT_FAILURE);
    }
    add_to_array(knownFiles, &filePath);
}

void remove_words_from_dictionary(const char *filepath, Array *dictionary) {
    for (size_t i = 0; i < dictionary->size; ) {
        WordPair *pair = (WordPair *)get_from_array(dictionary, i);

        if (strcmp(pair->sourceFile, filepath) == 0) {
            // Free memory for the word pair
            // free_wordpair(pair);
            // Remove the entry from the dictionary
            remove_from_array(dictionary, i);
        } else {
            i++; // Only increment if not removed
        }
    }
}

void remove_from_known_files(size_t index, Array *knownFiles) {
    if (index >= knownFiles->size) {
        fprintf(stderr, "Index out of bounds in remove_from_known_files\n");
        return;
    }

    // Free the memory for the file path
    char *filePath = *(char **)get_from_array(knownFiles, index);
    free(filePath);

    // Remove the entry from the array
    remove_from_array(knownFiles, index);
}

// Function to free KnownFiles
void free_known_files(Array *knownFiles) {
    for (size_t i = 0; i < knownFiles->size; i++) {
        char *filePath = *(char **)get_from_array(knownFiles, i);
        free(filePath);
    }
}

void print_dictionary(Array *dictionary) {
    for (size_t i = 0; i < dictionary->size; i++) {
        WordPair *pair = (WordPair *)get_from_array(dictionary, i);
        printf("WordPair %zu: %s -> %s\n", i, pair->english, pair->french);
    }
}

void print_known_files(Array *knownFiles) {
    for (size_t i = 0; i < knownFiles->size; i++) {
        char *filePath = *(char **)get_from_array(knownFiles, i);  // Get the file path from the array
        printf("Known File %zu: %s\n", i, filePath);
    }
}
