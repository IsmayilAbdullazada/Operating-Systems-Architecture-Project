#ifndef READER_THREAD_H
#define READER_THREAD_H
// Structure to pass both `msgid` and `shmid` to threads
typedef struct {
    int msgid;
    int shmid;
} ThreadArgs;

#endif // READER_THREAD_H

void *reader_thread(void *arg);
