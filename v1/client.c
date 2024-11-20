#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server_pid>\n", argv[0]);
        return 1;
    }

    pid_t server_pid = atoi(argv[1]);
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int signal_to_send = (rand() % 2 == 0) ? SIGUSR1 : SIGUSR2;
        kill(server_pid, signal_to_send);
        usleep(200000); // Sleep for 10 milliseconds (10,000 microseconds)
    }

    return 0;
}
