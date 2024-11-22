#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "writer_thread.h"
#include "reader_thread.h"
#include "message.h"
#include "shared.h"
#include <sys/shm.h>

volatile sig_atomic_t terminate_flag = 0;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\nSIGINT received, terminating program...\n");
        terminate_flag = 1;
    }
}

int main() {
    signal(SIGINT, handle_signal);

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

    ThreadArgs reader_args = {msgid, shmid};

    if (pthread_create(&writer, NULL, writer_thread, &msgid) != 0) {
        perror("pthread_create (writer)");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&reader, NULL, reader_thread, &reader_args) != 0) {
        perror("pthread_create (reader)");
        exit(EXIT_FAILURE);
    }

    pthread_join(writer, NULL);

    terminate_flag = 1;
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl (IPC_RMID)");
    }

    pthread_join(reader, NULL);

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl (IPC_RMID)");
    }

    printf("Program terminated.\n");

    return 0;
}
