#ifndef DICTIONARY_LOADER_H
#define DICTIONARY_LOADER_H

#include "array.h"
#include "wordpair.h"

#ifndef PATH_MAX
#define PATH_MAX 1012  // Maximum path length
#endif

#define DICT_RELOAD_INTERVAL_SEC 1  // Interval for reloading the dictionary in seconds

// Load all word pairs from a file into a dictionary
void load_file(const char *filepath, Array *dictionary);

void load_dictionary(const char *folder_path, Array *dictionary, Array *knownFiles);

// Check if a file is already known
int is_known_file(const char *filename, Array *knownFiles);

// Add a file to the list of known files
void add_to_known_files(const char *filename, Array *knownFiles);

// Remove words from the dictionary for a given file
void remove_words_from_dictionary(const char *filepath, Array *dictionary);

// Remove a file from the known files list
void remove_from_known_files(size_t index, Array *knownFiles);

void free_known_files(Array *knownFiles);

void print_dictionary(Array *dictionary);

void print_known_files(Array *knownFiles);

#endif
