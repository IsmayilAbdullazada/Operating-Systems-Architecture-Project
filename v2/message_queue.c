#include "message_queue.h"
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int msg_id;

void init_message_queue() {
    msg_id = msgget(MSG_QUEUE_KEY, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("Failed to create message queue");
        exit(EXIT_FAILURE);
    }
}

void cleanup_message_queue() {
    if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
        perror("Failed to remove message queue");
    }
}

int send_message(Message *msg) {
    // printf("Sending message with eng: %s fr: %s\n", msg->pair.english, msg->pair.french);
    return msgsnd(msg_id, msg, sizeof(Message) - sizeof(long), 0);
}

int receive_message(Message *msg) {
    // printf("Receiving message with eng: %s fr: %s\n", msg->pair.english, msg->pair.french);
    return msgrcv(msg_id, msg, sizeof(Message) - sizeof(long), 0, 0);
}
