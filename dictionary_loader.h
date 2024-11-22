#ifndef DICTIONARY_LOADER_H
#define DICTIONARY_LOADER_H

#include "array.h"
#include "wordpair.h"

#ifndef PATH_MAX
#define PATH_MAX 1012
#endif

#define DICT_RELOAD_INTERVAL_SEC 3

void load_file(const char *filepath, Array *dictionary);
void load_dictionary(const char *folder_path, Array *dictionary, Array *knownFiles);

#endif
