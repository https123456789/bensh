#include <stdio.h>
#include <signal.h>

#include "sigdefs.h"

// The SIGINT handler simply needs to be blank so that the signal gets passed to the child program
static void sigint_handler(int signum, siginfo_t *info, void *context) {
}

int init_signal_handlers() {
    struct sigaction action;
    action.sa_sigaction = sigint_handler;

    if (sigemptyset(&action.sa_mask) < 0) {
        perror("sigemptyset");
        return -1;
    }
    action.sa_flags = SA_RESTART | SA_SIGINFO;

    if (sigaction(SIGINT, &action, NULL) < 0) {
        perror("sigaction");
        return -1;
    }

    return 0;
}
