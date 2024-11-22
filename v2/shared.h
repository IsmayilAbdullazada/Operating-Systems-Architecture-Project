#ifndef SHARED_H
#define SHARED_H

#include <signal.h>

extern volatile sig_atomic_t terminate_flag;
extern pthread_rwlock_t array_rwlock;

#endif
