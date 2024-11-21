#ifndef SHARED_H
#define SHARED_H

#include <signal.h>

// Termination flag for graceful shutdown
extern volatile sig_atomic_t terminate_flag;

#endif // SHARED_H
