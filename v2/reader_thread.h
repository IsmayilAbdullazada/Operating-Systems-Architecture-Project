#ifndef READER_THREAD_H
#define READER_THREAD_H

typedef struct {
    int msgid;
    int shmid;
} ThreadArgs;

#endif

void *reader_thread(void *arg);
