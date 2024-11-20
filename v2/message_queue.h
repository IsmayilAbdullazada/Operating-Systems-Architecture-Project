#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "message.h"

void init_message_queue();
void cleanup_message_queue();
int send_message(Message *msg);
int receive_message(Message *msg);

#endif
