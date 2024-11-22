#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include "../v2/writer_thread.h"
#include "../v2/reader_thread.h"
#include "../v2/message.h"
#include "../v2/shared.h"
#include "../v2/shared_array.h"

volatile sig_atomic_t terminate_flag = 0; // Termination flag for clean shutdown

SharedArray *dictionary; // Shared array for WordPairs

// Thread function for signal handling
void *signal_handler_thread(void *arg) {
    int sig = *(int *)arg;
    free(arg);

    if (terminate_flag) {
        pthread_exit(NULL);
    }

    if (sig == SIGINT) {
        printf("\nSIGINT received, terminating program...\n");
        terminate_flag = 1;
        pthread_exit(NULL);
    }


    pthread_rwlock_rdlock(&array_rwlock);

    if (dictionary->size == 0) {
        printf("No word pairs available in the dictionary.\n");
        pthread_rwlock_unlock(&array_rwlock); 
        pthread_exit(NULL);
    }

    int randomIndex = rand() % dictionary->size;
    WordPair_shared *pair = &dictionary->array[randomIndex];

    if (sig == SIGUSR1) {
        printf("English: %s, French: %s\n", pair->english, pair->french);
    } else if (sig == SIGUSR2) {
        printf("French: %s, English: %s\n", pair->french, pair->english);
    }

    pthread_rwlock_unlock(&array_rwlock);

    pthread_exit(NULL);
}


// Signal handler
void signal_handler(int sig) {
    pthread_t signal_thread;
    int *arg = malloc(sizeof(int));
    if (arg == NULL) {
        perror("malloc");
        return;
    }

    *arg = sig;

    if (pthread_create(&signal_thread, NULL, signal_handler_thread, arg) != 0) {
        perror("pthread_create");
        free(arg);
        return;
    }

    pthread_detach(signal_thread);
}


pthread_rwlock_t array_rwlock; 



int main() {
    printf("Server process ID: %d\n", getpid());
    
    if (pthread_rwlock_init(&array_rwlock, NULL) != 0) {
        perror("pthread_rwlock_init");
        exit(EXIT_FAILURE);
    }
    
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    pthread_t writer, reader;


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

    if (pthread_rwlock_destroy(&array_rwlock) != 0) {
        perror("pthread_rwlock_destroy");
    }

    printf("Program terminated.\n");
    return 0;
}