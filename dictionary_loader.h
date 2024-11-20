#ifndef DICTIONARY_LOADER_H
#define DICTIONARY_LOADER_H

#include "array.h"
#include "wordpair.h"

#ifndef PATH_MAX
#define PATH_MAX 1012  // Maximum path length
#endif

#define DICT_RELOAD_INTERVAL_SEC 5  // Interval for reloading the dictionary in seconds

// Load all word pairs from a file into a dictionary
void load_file(const char *filepath, Array *dictionary);

void load_dictionary(const char *folder_path, Array *dictionary, Array *knownFiles);

void *dictionary_loader(void *arg, Array *dictionary, Array *knownFiles);

// Check if a file is already known
int is_known_file(const char *filename, Array *knownFiles);

// Add a file to the list of known files
void add_to_known_files(const char *filename, Array *knownFiles);



void free_file_path(void *element);

#endif
