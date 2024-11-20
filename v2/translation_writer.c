#include "../dictionary_loader.h"
#include "../array.h"
#include "message.h"
#include "message_queue.h"
#include <unistd.h>
#include <stdio.h>

void *translation_writer(void *arg) {
    const char *folder = (const char *)arg;

    // Initialize known files list
    Array knownFiles;
    init_array(&knownFiles, sizeof(char *), 10);

    // Periodically scan the folder for new files
    while (1) {
        // Use `load_dictionary` to handle file scanning
        Array tempDictionary;
        init_array(&tempDictionary, sizeof(WordPair), 100);

        // Log folder scanning activity
        printf("Writer: Scanning folder for new dictionary files...\n");

        // Simulate loading dictionary from files (add sleep to simulate delay)
        load_dictionary(folder, &tempDictionary, &knownFiles);

        // Process the word pairs into messages
        for (size_t i = 0; i < tempDictionary.size; i++) {
            WordPair *pair = (WordPair *)get_from_array(&tempDictionary, i);

            // Create a message
            Message msg;

            // Randomly decide whether it's English-to-French (msg_type = 1) or French-to-English (msg_type = 2)
            if (rand() % 2 == 0) {
                msg.msg_type = 1; // English-to-French
            } else {
                msg.msg_type = 2; // French-to-English
            }
            msg.pair.english = strdup(pair->english);
            msg.pair.french = strdup(pair->french);

            // Log the message about to be sent
            printf("Writer: Sending message: '%s' -> '%s' (Direction: %s)\n", 
                   msg.pair.english, msg.pair.french, 
                   (msg.msg_type == 1 ? "English-to-French" : "French-to-English"));

            // Send the message to the queue
            send_message(&msg);

            // Sleep after sending each message (simulating delay in processing)
            sleep(1); // Adjust as needed
        }

        // Free temporary dictionary storage
        free_array(&tempDictionary);

        // Wait for the next scan interval
        printf("Writer: Waiting for the next scan interval...\n");
        sleep(DICT_RELOAD_INTERVAL_SEC); // Sleep to simulate delay between folder scans
    }

    // Free resources used for tracking known files
    free_known_files(&knownFiles);

    return NULL;
}
