#ifndef DICTIONARY_LOADER_H
#define DICTIONARY_LOADER_H

#include "array.h"
#include "wordpair.h"

#ifndef PATH_MAX
#define PATH_MAX 1012  // Maximum path length
#endif

#define DICT_RELOAD_INTERVAL_SEC 3  // Interval for reloading the dictionary in seconds

// Load all word pairs from a file into a dictionary
void load_file(const char *filepath, Array *dictionary);

void load_dictionary(const char *folder_path, Array *dictionary, Array *knownFiles);

void free_file_path(void *element);

// void print_dictionary(Array *dictionary);

// void print_known_files(Array *knownFiles);

#endif
