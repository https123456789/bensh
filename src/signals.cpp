// Copyright 2023 Ben Landon

#include <setjmp.h>
#include <signal.h>
#include <iostream>
#include "bensh.hpp"

extern pid_t childPid;
extern sigjmp_buf env;
extern volatile sig_atomic_t jump_active;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    _Exit(signum);
}

void signalIntHandler(int signum) {
    if (!jump_active) {
        return;
    }
    if (childPid) {
        // Send the interupt to the child
        kill(childPid, SIGINT);
    }
    siglongjmp(env, 42);
}
