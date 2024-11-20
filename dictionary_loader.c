#include "dictionary_loader.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "obj_string.h"

void load_file(const char *filepath, Array *dictionary) {
    FILE *file = fopen(filepath, "r");
    if (file != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL) {
            char *english = strtok(line, ";");
            char *french = strtok(NULL, "\n");

            if (english && french) {
                WordPair* wp = WordPair_new(english, french, filepath);
                
                Array_add(dictionary, (Object*) wp);
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
    String *str;

    if ((dir = opendir(folder_path)) != NULL) {
        // Load new files from the folder
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            // printf("%s\n", ent->d_name);
            snprintf(filepath, PATH_MAX, "%s/%s", folder_path, ent->d_name);
            
            str = String_new(filepath);

            if (!Array_in(knownFiles, (Object *)str)) {
                printf("Loading new file: %s\n", filepath);
                load_file(filepath, dictionary);
                Array_add(knownFiles, (Object *)str);
            } else {
                String_free((Object*) str);
            }
        }
        closedir(dir);

        // Check for removed files
        for (size_t i = 0; i < knownFiles->size; ) {
            Object *knownFile = Array_get(knownFiles, i);
            const char* knownFileString = knownFile->to_string(knownFile);

            // If the file no longer exists, remove it
            if (access(knownFileString, F_OK) != 0) {
                printf("File removed: %s\n", knownFileString);

                // Remove words from the dictionary associated with the file
                Array_filter(dictionary, cmpSourceFiles, knownFile);

                // Remove from knownFiles
                Array_remove_at(knownFiles, i);

            } else {
                i++; // Only increment if not removed
            }
        }
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }

    const char *dictionaryString = Array_to_string(dictionary);
    printf("%s\n", dictionaryString);
    printf("\n" );
    free((void *)dictionaryString);
}

// Thread function to periodically load the dictionary
void *dictionary_loader(void *arg, Array *dictionary, Array *knownFiles) {
    const char *folder = (const char *)arg;
    while (1) {
        load_dictionary(folder, dictionary, knownFiles);  // Load the dictionary
        sleep(DICT_RELOAD_INTERVAL_SEC);  // Wait for the specified interval
    }
    return NULL;  // Not used but required by pthread
}

int is_known_file(const char *filename, Array *knownFiles) {
    for (size_t i = 0; i < knownFiles->size; i++) {
        char *knownFile = *(char **)Array_get(knownFiles, i); 
        if (strcmp(knownFile, filename) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_to_known_files(const char *filename, Array *knownFiles) {
    String *filePath = String_new(filename);
    Array_add(knownFiles, (Object *)filePath);
}


void free_file_path(void *element) {
    char *filePath = *(char **)element; // Dereference to get the file path pointer
    free(filePath); // Free the dynamically allocated file path
}



