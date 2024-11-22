#include "dictionary_loader.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "./objects/obj_string.h"

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
            } else {
                fprintf(stderr, "Invalid line in file: %s\n", line);

            }
        }
        fclose(file);
    } else {
        perror("Error opening file");
    }
}

void load_dictionary(const char *folder_path, Array *dictionary, Array *knownFiles) {
    DIR *dir;
    struct dirent *ent;
    char filepath[PATH_MAX];
    String *str;

    if ((dir = opendir(folder_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
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

        for (size_t i = 0; i < knownFiles->size; ) {
            Object *knownFile = Array_get(knownFiles, i);
            const char* knownFileString = knownFile->to_string(knownFile);

            if (access(knownFileString, F_OK) != 0) {
                printf("File removed: %s\n", knownFileString);

                Array_filter(dictionary, cmpSourceFiles, knownFileString);
                Array_remove_at(knownFiles, i);
            } else {
                i++; // Only increment if not removed
            }
        }
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }
}