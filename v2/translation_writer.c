#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "dictionary.h"
#include "message_queue.h"

void *translation_writer(void *arg) {
    const char *folder = (const char *)arg;

    while (1) {
        DIR *dir = opendir(folder);
        struct dirent *ent;
        char filepath[PATH_MAX];

        if (dir == NULL) {
            perror("Failed to open directory");
            break;
        }

        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;

            snprintf(filepath, PATH_MAX, "%s/%s", folder, ent->d_name);
            FILE *file = fopen(filepath, "r");
            if (!file) {
                perror("Error opening file");
                continue;
            }

            char line[1024];
            while (fgets(line, sizeof(line), file)) {
                char *english = strtok(line, ";");
                char *french = strtok(NULL, "\n");
                if (english && french) {
                    Message msg;
                    msg.msg_type = rand() % 2 == 0 ? 1 : 2; // Random direction
                    msg.pair.english = strdup(english);
                    msg.pair.french = strdup(french);
                    send_message(&msg);
                }
            }
            fclose(file);
        }
        closedir(dir);

        sleep(DICT_RELOAD_INTERVAL_SEC); // Wait for the next scan
    }

    return NULL;
}
