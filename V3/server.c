#include <stdio.h>        // For printf(), perror(), etc.
#include <stdlib.h>       // For exit(), malloc(), free(), etc.
#include <string.h>       // For string manipulation functions like strcpy(), strcmp(), etc.
#include <pthread.h>      // For pthread_t, pthread_create(), pthread_join(), etc.
#include <sys/ipc.h>      // For IPC_PRIVATE, IPC_CREAT (System V IPC).
#include <sys/msg.h>      // For msgget(), msgsnd(), msgrcv(), msgctl().
#include <unistd.h>       // For sleep(), access().
#include <errno.h>        // For error handling and errno.
#include <dirent.h>       // For directory operations (opendir(), readdir(), closedir()).
#include "../v2/writer_thread.h"  // For writer_thread definition.
#include "../v2/reader_thread.h"  // For reader_thread definition.
#include "../v2/message.h"        // For Message structure definition.
#include "../v2/shared.h"
#include "../v2/shared_array.h"
#include <sys/shm.h>

volatile sig_atomic_t terminate_flag = 0;  // Define the termination flag


SharedArray *dictionary;  // Shared array for WordPairs

// Signal handler for SIGINT
void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\nSIGINT received, terminating program...\n");
        terminate_flag = 1;
    }
    
    if (dictionary->size == 0) {
        printf("No word pairs available in the dictionary.\n");
        return;
    }

    int randomIndex = rand() % dictionary->size;
    WordPair_shared *pair = &dictionary->array[randomIndex];

    if (sig == SIGUSR1) {
        printf("English: %s, French: %s\n", pair->english, pair->french);
    } else if (sig == SIGUSR2) {
        printf("French: %s, English: %s\n", pair->french, pair->english);
    }
}


int main() {
    // print process ID
    printf("Server process ID: %d\n", getpid());
    // Set up signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    pthread_t writer, reader;

    // Create a message queue
    key_t msgkey = ftok("/tmp", 42);
    int msgid = msgget(msgkey, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Message queue ID: %d\n", msgid);

    // Create shared memory for the shared array
    key_t shmkey = ftok("/tmp", 65);
    int shmid = shmget(shmkey, 10 * 1024 * 1024, 0666 | IPC_CREAT); // 10 MB shared memory
    if (shmid == -1) {
        perror("shmget");
        msgctl(msgid, IPC_RMID, NULL); // Clean up message queue on failure
        exit(EXIT_FAILURE);
    }
    printf("Shared memory ID: %d\n", shmid);

    // Attach to shared memory
    dictionary = (SharedArray *)shmat(shmid, NULL, 0);
    if (dictionary == (SharedArray *)-1) {
        perror("shmat");
        msgctl(msgid, IPC_RMID, NULL); // Clean up message queue
        shmctl(shmid, IPC_RMID, NULL); // Clean up shared memory
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory structure
    dictionary->size = 0;
    dictionary->capacity = (10 * 1024 * 1024 - sizeof(SharedArray)) / sizeof(WordPair_shared);

    printf("Shared memory attached. Dictionary capacity: %zu\n", dictionary->capacity);

    // Prepare thread arguments
    ThreadArgs reader_args = {msgid, shmid};

    // Start the writer thread
    if (pthread_create(&writer, NULL, writer_thread, &msgid) != 0) {
        perror("pthread_create (writer)");
        msgctl(msgid, IPC_RMID, NULL); // Clean up message queue
        shmctl(shmid, IPC_RMID, NULL); // Clean up shared memory
        exit(EXIT_FAILURE);
    }

    // Start the reader thread
    if (pthread_create(&reader, NULL, reader_thread, &reader_args) != 0) {
        perror("pthread_create (reader)");
        msgctl(msgid, IPC_RMID, NULL); // Clean up message queue
        shmctl(shmid, IPC_RMID, NULL); // Clean up shared memory
        exit(EXIT_FAILURE);
    }

    // Wait for writer thread to finish
    pthread_join(writer, NULL);

    // Set termination flag and remove the message queue
    terminate_flag = 1;
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl (IPC_RMID)");
    }

    // Wait for reader thread to finish
    pthread_join(reader, NULL);

    // Detach from shared memory
    if (shmdt(dictionary) == -1) {
        perror("shmdt");
    }

    // Clean up shared memory
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl (IPC_RMID)");
    }

    printf("Program terminated.\n");

    return 0;
}
