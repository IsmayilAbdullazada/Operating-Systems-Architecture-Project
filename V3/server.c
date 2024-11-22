#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "../v2/writer_thread.h"
#include "../v2/reader_thread.h"
#include "../v2/message.h"
#include "../v2/shared.h"
#include "../v2/shared_array.h"

volatile sig_atomic_t terminate_flag = 0;  // Define the termination flag

SharedArray *dictionary;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\nSIGINT received, terminating program...\n");
        terminate_flag = 1;
        return;
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

    key_t shmkey = ftok("/tmp", 65);
    int shmid = shmget(shmkey, 10 * 1024 * 1024, 0666 | IPC_CREAT); // 10 MB shared memory
    if (shmid == -1) {
        perror("shmget");
        msgctl(msgid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    printf("Shared memory ID: %d\n", shmid);

    dictionary = (SharedArray *)shmat(shmid, NULL, 0);
    if (dictionary == (SharedArray *)-1) {
        perror("shmat");
        msgctl(msgid, IPC_RMID, NULL);
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    dictionary->size = 0;
    dictionary->capacity = (10 * 1024 * 1024 - sizeof(SharedArray)) / sizeof(WordPair_shared);

    printf("Shared memory attached. Dictionary capacity: %zu\n", dictionary->capacity);

    ThreadArgs reader_args = {msgid, shmid};

    if (pthread_create(&writer, NULL, writer_thread, &msgid) != 0) {
        perror("pthread_create (writer)");
        msgctl(msgid, IPC_RMID, NULL);
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&reader, NULL, reader_thread, &reader_args) != 0) {
        perror("pthread_create (reader)");
        msgctl(msgid, IPC_RMID, NULL);
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    pthread_join(writer, NULL);

    terminate_flag = 1;
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl (IPC_RMID)");
    }

    pthread_join(reader, NULL);

    if (shmdt(dictionary) == -1) {
        perror("shmdt");
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl (IPC_RMID)");
    }

    printf("Program terminated.\n");

    return 0;
}
