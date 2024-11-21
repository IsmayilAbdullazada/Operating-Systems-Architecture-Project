#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "shared_memory.h"
#include "message_queue.h"
#include "message.h"
#include "../array.h"
#include "../wordpair.h"
#include "translation_writer.c"
#include "translation_reader.c"

int main() {
    Array dictionary;
    init_shared_memory(&dictionary, sizeof(WordPair), 1024); // Shared memory setup
    init_message_queue();

    pthread_t writer_thread, reader_thread;

    if (pthread_create(&writer_thread, NULL, translation_writer, (void *)"../dictionary_folder") != 0) {
        perror("Failed to create writer thread");
        return 1;
    }

    if (pthread_create(&reader_thread, NULL, translation_reader, &dictionary) != 0) {
        perror("Failed to create reader thread");
        return 1;
    }

    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    cleanup_shared_memory();
    cleanup_message_queue();

    return 0;
}
